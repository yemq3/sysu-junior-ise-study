#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "task.h"
#include "cpu.h"
#include "schedulers.h"

struct node *list;
int i = 0;

// add a task to the list 
void add(char *name, int priority, int burst){
    Task *t = malloc(sizeof(Task));
    t->name = name;
    t->priority = priority;
    t->burst = burst;
    t->tid = i++;
    insert(&list,t);
}

// invoke the scheduler
void schedule(){
    while(list){
        run(list->task, list->task->burst);
        delete(&list,list->task);
    }
}
