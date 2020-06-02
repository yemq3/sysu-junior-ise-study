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
    int timeQuantum = 10;
    struct node *temp;

    while(list){
        temp = list;
        while(temp){
            int remainTime = temp->task->burst - timeQuantum;
            if(remainTime > 0){
                run(temp->task, timeQuantum);
                temp->task->burst -= timeQuantum;
            }
            else{
                run(temp->task, temp->task->burst);
                delete(&list,temp->task);
            }
            temp = temp->next;
        }
    }
}
