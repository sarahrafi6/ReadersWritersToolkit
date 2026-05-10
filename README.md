this project implements a Readers-Writers Toolkit that demonstrates correct synchronization of concurrent access to a shared resource in Linux. 
The solution uses POSIX semaphores to implement the reader-priority Readers-Writers algorithm, supported by a Linux kernel module and 
a GTK-based graphical interface for visual demonstration.
System Architecture:
Kernel Layer: rw_module.ko initializes kernel semaphores using sema_init.
User Layer: Multi-threaded program with reader and writer threads accessing a shared integer.
GUI Layer: GTK3 application with writer and reader panels demonstrating real-time synchronization.
