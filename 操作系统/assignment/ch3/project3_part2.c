#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>


void dfs(struct task_struct *task){
    struct task_struct *child;
    struct list_head *list;

    printk( KERN_INFO "command = [%s] pid = [%d] state = [%ld]",task->comm,task->pid,task->state);

    list_for_each(list, &task->children){
        child = list_entry(list, struct task_struct, sibling);
        dfs(child);
    }
}

/* This function is called when the module is loaded. */
int proc_init(void)
{
    printk( KERN_INFO "init");

    dfs(&init_task);
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