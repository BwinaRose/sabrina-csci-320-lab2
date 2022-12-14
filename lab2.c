#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab2.h"


int** read_board_from_file(char* filename){
    int** sudoku_board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    for(int row = 0; row < ROW_SIZE; row++){
	    sudoku_board[row] = (int*)malloc(sizeof(int)*COL_SIZE);
    } 
    FILE *fp = NULL;
    fp = fopen(filename,"r");
    //fscan the board into var
    fclose(fp);
    return sudoku_board;
}
//create functions to validate rows, cols, and subarrays(each sudoku square)? (possible within the validate board function?)

int is_board_valid(void *data){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    // replace this comment with your code
    //create threads to validate
    
}