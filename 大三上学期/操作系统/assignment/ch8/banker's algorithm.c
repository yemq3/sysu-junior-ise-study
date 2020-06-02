#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define true 1
#define false 0

/* the available amount of each resource */ 
int available[NUMBER_OF_RESOURCES];

/*the maximum demand of each customer */ 
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */ 
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */ 
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int find(int work[], int finish[]){
    int i, j;
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        if(finish[i] == false){
            for(j = 0; j < NUMBER_OF_RESOURCES; j++){
                if(need[i][j] > work[j]){
                    break;
                }
                if(need[i][j] <= work[j] && j == NUMBER_OF_RESOURCES - 1){
                    return i;
                }
            }
        }
    }
    return -1;
}

int safety(){
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS] = {false};
    memcpy(work, available, sizeof(available));

    int customer_num, i;
    while(true){
        customer_num = find(work, finish);
        if(customer_num >= 0){
            for(i = 0; i < NUMBER_OF_RESOURCES; i++){
                work[i] = work[i] + allocation[customer_num][i];
                finish[i] = true;
            }
        }
        else{
            break;
        }
    }

    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        if(finish[i] == false)
            return false;
    }
    return true;
}

int request_resources(int customer_num, int request[]){
    int i;
    // step 1
    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        if(request[i] > need[customer_num][i]){
            return -1;
        }
    }

    // step 2
    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        if(request[i] > available[i]){
            return -2;
        }
    }

    // step3
    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] = available[i] - request[i];
        allocation[customer_num][i] = allocation[customer_num][i] + request[i];
        need[customer_num][i] = need[customer_num][i] - request[i];
    }
    if(safety()){
        return 0;
    }
    else{
        for(i = 0; i < NUMBER_OF_RESOURCES; i++){
            available[i] = available[i] + request[i];
            allocation[customer_num][i] = allocation[customer_num][i] - request[i];
            need[customer_num][i] = need[customer_num][i] + request[i];
        }
        return -3;
    }

    return 0;
}

void release_resources(int customer_num, int release[]){
    int i;
    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        if(release[i] > allocation[customer_num][i])
            return ;
    }
    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] = available[i] + release[i];
        allocation[customer_num][i] = allocation[customer_num][i] - release[i];
        need[customer_num][i] = need[customer_num][i] + release[i];
    }
}


void print_data(){
    int i, j;
    printf("available:\n");
    for(i = 0; i < NUMBER_OF_RESOURCES;  i++){
        printf("%d ", available[i]);
    }
    printf("\n");

    printf("maximun:\n");
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        for(j = 0; j < NUMBER_OF_RESOURCES; j++){
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }

    printf("allocation:\n");
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        for(j = 0; j < NUMBER_OF_RESOURCES; j++){
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    printf("need:\n");
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        for(j = 0; j < NUMBER_OF_RESOURCES; j++){
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]){

    FILE *maxData = fopen("maximun.txt","r");
    char data[100];
    char *temp;
    int i;
    int customer_num = 0;
    char command[10];

    while (fgets(data,100,maxData) != NULL) {
        temp = strdup(data);
        for(i = 0; i < 4; i++){
            int num = atoi(strsep(&temp, ","));
            maximum[customer_num][i] = num;
            need[customer_num][i] = num;
        }
        customer_num++;
    }

    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] = atoi(argv[i+1]);
    }

    while(true){
        scanf("%s", command);
        if(strcmp(command, "*") == 0){
            print_data();
            return 0;
        }
        else if(strcmp(command, "RL") == 0){
            scanf("%d", &customer_num);
            int request[4];
            for(i = 0; i < 4; i++){
                scanf("%d", &request[i]);
            }
            release_resources(customer_num, request);
        }
        else if(strcmp(command, "RQ") == 0){
            scanf("%d", &customer_num);
            int release[4];
            for(i = 0; i < 4; i++){
                scanf("%d", &release[i]);
            }
            int code = request_resources(customer_num, release);
            if(code < 0){
                printf("error code: %d\n", code);
            }
        }
    }
}