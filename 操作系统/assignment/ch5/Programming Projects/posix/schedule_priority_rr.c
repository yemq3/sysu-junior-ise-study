#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "list.h"
#include "task.h"
#include "cpu.h"
#include "schedulers.h"

struct node *list[10];
int i = 0;

// add a task to the list 
void add(char *name, int priority, int burst){
    Task *t = malloc(sizeof(Task));
    t->name = name;
    t->priority = priority;
    t->burst = burst;
    t->tid = i++;
    insert(&list[priority],t);
}

// invoke the scheduler
void schedule(){
    int timeQuantum = 10;
    struct node *temp;
    int p;

    for(p = 10; p > 0; p--){
        while(list[p]){
            temp = list[p];
            while(temp){
                int remainTime = temp->task->burst - timeQuantum;
                if(remainTime > 0){
                    run(temp->task, timeQuantum);
                    temp->task->burst -= timeQuantum;
                }
                else{
                    run(temp->task, temp->task->burst);
                    delete(&list[p],temp->task);
                }
                temp = temp->next;
            }
        }
    }
    
}
