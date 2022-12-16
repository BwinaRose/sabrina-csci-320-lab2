#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab2.h"

int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** sudoku_board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    for(int row = 0; row < ROW_SIZE; row++){
	    sudoku_board[row] = (int*)malloc(sizeof(int)*COL_SIZE);
    }

    fp = fopen(filename,"r");
    for(int i = 0; i < ROW_SIZE; i++) {
        for(int j = 0; j < COL_SIZE; j++) {
            fscanf(fp, "%d%*c", &sudoku_board[i][j]);
        }
    }   
    fclose(fp);
    return sudoku_board;
}

void * validate_row(void * params)
{
    param_struct * param = (param_struct*) params;
    int verifyArr[9] = {0};
    int row = param -> starting_row;
    int col = param -> starting_col;
    
	int i;
	for (i = 0; i < 9; i++) {
		int num = sudoku_board[row][i];
		if (num < 1 || num > 9 || verifyArr[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			verifyArr[num - 1] = 1;		
		}
	}
	worker_validation[9 + row] = 1;
	pthread_exit(NULL);
}


void * validate_col(void* params)
{
    param_struct *p = (param_struct*) params;
    int validation_array[9] = {0};
    int row = p -> starting_row;
    int col = p -> starting_col;

    for(int i = 0; i< 9; i++)
    {   
        int num = sudoku_board[i][col];
        if (num > 9 || num < 1 || validation_array[num - 1]==1)
        {
            pthread_exit(NULL);
        }
        else{
            validation_array[num - 1] = 1;
        }
    } 

    worker_validation[18 + col] = 1;
    pthread_exit(NULL);
}

void * validate_subarray(void* params)
{
    param_struct *p = (param_struct*) params;
    int validation_array[9] = {0};
    int row = p -> starting_row;
    int col = p -> starting_col;

    for(int i = row; i < row + 3; i++){
        for(int j = col; j < col + 3; j++)
        {
            int num = sudoku_board[i][j];
            if (num > 9 || num < 1 || validation_array[num - 1]==1)
            {
                pthread_exit(NULL);
            }
            else{
                validation_array[num - 1] = 1;
            }
        }
    }
    worker_validation[row + col / 3] = 1;
    pthread_exit(NULL);

}

int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    int t_index = 0;
    worker_validation = (int*) malloc(sizeof(int) * NUM_OF_THREADS);
    tid = (pthread_t*) malloc(sizeof(int) * NUM_OF_THREADS);

    for(int i = 0; i < ROW_SIZE; i++)
    {
        for(int j = 0; j < COL_SIZE; j++)
        {
            if(i % 3 == 0 && j % 3 == 0){
                param_struct * subarray_workthread = (param_struct*) malloc(sizeof(param_struct));
                subarray_workthread->starting_row = i;
                subarray_workthread->starting_col = j;
                pthread_create(&tid[t_index++], NULL, validate_subarray, subarray_workthread);
            }
            if(i==0){
                param_struct * col_workthread = (param_struct*) malloc(sizeof(param_struct));
                col_workthread->starting_row = i;
                col_workthread->starting_col = j;
                pthread_create(&tid[t_index++], NULL, validate_col,col_workthread);
            }
            if(j==0){
                param_struct * row_workthread = (param_struct*) malloc(sizeof(param_struct));
                row_workthread->starting_row = i;
                row_workthread->starting_col = j;
                pthread_create(&tid[t_index++], NULL, validate_row, row_workthread);
            }
            
        }
    }
    
    for(int i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
        if(worker_validation[i] == 0){
            return 0;
        }
    }

    free(worker_validation);
    free(tid);
    return 1;
}