#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

struct task_struct *task;
struct list_head *list;

/* This function is called when the module is loaded. */
int proc_init(void)
{
    for_each_process(task){
        /* on each iteration task points to the next task */
        printk( KERN_INFO "command = [%s] pid = [%d] state = [%ld]",task->comm,task->pid,task->state);
    }
	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {
    printk( KERN_INFO "exit");
}

module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux Kernel Module for Listing Tasks");
MODULE_AUTHOR("Clicker");