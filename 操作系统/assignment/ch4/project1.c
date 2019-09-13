#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 27

struct param{
    int i;
    int (*board)[9];
};

int valid[NUM_THREADS] = {0};

void *checkGrid(void *param);
void *checkRows(void *param);
void *checkCols(void *param);

int main(int argc, char *argv[]){
    int sudoku[9][9] =
    {
        {6, 2, 4, 5, 3, 9, 1, 8, 7},
        {5, 1, 9, 7, 2, 8, 6, 3, 4},
        {8, 3, 7, 6, 1, 4, 2, 9, 5},
        {1, 4, 3, 8, 6, 5, 7, 2, 9},
        {9, 5, 8, 2, 4, 7, 3, 6, 1},
        {7, 6, 2, 3, 9, 1, 4, 5, 8},
        {3, 7, 1, 9, 5, 6, 8, 4, 2},
        {4, 9, 6, 1, 8, 2, 5, 7, 3},
        {2, 8, 5, 4, 7, 3, 9, 1, 6}
     };
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;
    int i;

    pthread_attr_init(&attr);
    for(i = 0; i < 9; i++){
        struct param *p = (struct param *)malloc(sizeof(struct param));
        p->i = i;
        p->board = sudoku; 
        pthread_create(&tid[i], &attr, checkCols, p);
        pthread_create(&tid[i+9], &attr, checkRows, p);
        pthread_create(&tid[i+18], &attr, checkGrid, p);
    }
    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);

    for(int i = 0; i < NUM_THREADS; i++){
        if(valid[i] == 1){
            continue;
        }
        else{
            printf("Sudoku puzzle isn't valid.\n");
            return 0;
        }
    }
    printf("Sudoku puzzle is valid.\n");
        
    return 0;
}

void *checkCols(void *param){
    struct param *p = (struct param*)param;
    int i, val;
    int cols = p->i;
    int hash[9] = {0};
    for(i = 0; i < 9; i++){
        val = p->board[i][cols];
        if(hash[val-1] == 0){
            hash[val-1] = 1;
        }
        else{
            pthread_exit(0);
        }
    }
    valid[cols] = 1;
    pthread_exit(0);
}

void *checkRows(void *param){
    struct param *p = (struct param*)param;
    int i, val;
    int rows = p->i;
    int hash[9] = {0};
    for(i = 0; i < 9; i++){
        val = p->board[rows][i];
        if(hash[val-1] == 0){
            hash[val-1] = 1;
        }
        else{
            pthread_exit(0);
        }
    }
    valid[rows+9] = 1;
    pthread_exit(0);
}

void *checkGrid(void *param){
    // from left to right, from up to down, the grid is numbered from 0 to 8
    struct param *p = (struct param*)param;
    int i, j, val;
    int grid = p->i;
    int startRows = (grid / 3) * 3;
    int startCols = (grid % 3) * 3;
    int hash[9] = {0};
    for(i = startRows; i < startRows + 3; i++){
        for(j = startCols; j < startCols + 3; j++){
            val = p->board[i][j];
            if(hash[val-1] == 0){
                hash[val-1] = 1;
            }
            else{
                pthread_exit(0);
            }
        }
    }
    valid[grid + 18] = 1;
    pthread_exit(0);
}