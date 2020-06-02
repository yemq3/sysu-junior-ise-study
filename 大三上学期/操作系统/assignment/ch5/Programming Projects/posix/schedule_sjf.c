#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

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
    int MIN = INT_MAX;
    Task *runTask;
    struct node *temp;

    while(list){
        temp = list;
        while (temp) {
            if(temp->task->burst < MIN){
                MIN = temp->task->burst;
                runTask = temp->task;
            }
            temp = temp->next;
        }
        run(runTask,runTask->burst);
        MIN = INT_MAX;
        delete(&list, runTask);
    }
    
}
