#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RW OS Group");
MODULE_DESCRIPTION("Readers-Writers Kernel Module");

static struct semaphore rw_mutex;
static struct semaphore mutex;
static int readers_count = 0;

static int __init rw_init(void) {
    sema_init(&rw_mutex, 1);
    sema_init(&mutex, 1);
    printk(KERN_INFO "RW Module Loaded\n");
    return 0;
}

static void __exit rw_exit(void) {
    printk(KERN_INFO "RW Module Unloaded\n");
}

module_init(rw_init);
module_exit(rw_exit);
