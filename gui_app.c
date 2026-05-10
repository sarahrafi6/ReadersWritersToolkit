#include <gtk/gtk.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

GtkWidget *reader_text, *writer_entry, *status_label;
sem_t rw_mutex, mutex;
int readers_count = 0;
char shared_message[256] = "No message yet.";

void *reader_thread(void *arg) {
    sem_wait(&mutex);
    readers_count++;
    if (readers_count == 1) sem_wait(&rw_mutex);
    sem_post(&mutex);

    char display[300];
    snprintf(display, sizeof(display), "Reader sees: %s\n", shared_message);
    gtk_text_buffer_insert_at_cursor(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(reader_text)),
        display, -1);

    sem_wait(&mutex);
    readers_count--;
    if (readers_count == 0) sem_post(&rw_mutex);
    sem_post(&mutex);

    return NULL;
}

void on_write_clicked(GtkButton *btn, gpointer data) {
    sem_wait(&rw_mutex);

    const char *msg = gtk_entry_get_text(GTK_ENTRY(writer_entry));
    strncpy(shared_message, msg, sizeof(shared_message));
    gtk_label_set_text(GTK_LABEL(status_label), "Writer wrote a message.");

    sem_post(&rw_mutex);
}

void on_read_clicked(GtkButton *btn, gpointer data) {
    pthread_t t;
    pthread_create(&t, NULL, reader_thread, NULL);
    pthread_detach(t);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Readers-Writers Toolkit");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    GtkWidget *lbl1 = gtk_label_new("Writer: Enter a message");
    writer_entry = gtk_entry_new();
    GtkWidget *write_btn = gtk_button_new_with_label("Write Message");
    g_signal_connect(write_btn, "clicked", G_CALLBACK(on_write_clicked), NULL);

    GtkWidget *lbl2 = gtk_label_new("Readers Output:");
    reader_text = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(reader_text), FALSE);
    GtkWidget *read_btn = gtk_button_new_with_label("Start Reader");
    g_signal_connect(read_btn, "clicked", G_CALLBACK(on_read_clicked), NULL);

    status_label = gtk_label_new("Status: Idle");

    gtk_box_pack_start(GTK_BOX(vbox), lbl1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), writer_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), write_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), lbl2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), reader_text, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), read_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), status_label, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    return 0;
}
