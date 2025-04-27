#ifndef BOARD_H
#define BOARD_H
#include <stdbool.h>
#include <time.h>

typedef struct {
    int **board;
    bool **visibility;
    int  **solutions;
    int size;
 } SudokuBoard;

bool check_new_number(int **board, int row, int col, int num, int size);
void fill_board(int **board,bool **visibility, int size,int row, int col,time_t time);
void count_solutions(int **board,bool **visibility,int **solutions,int size,int row,int col,int *solutions_counter,int solution_limit);
void set_one_solution(int **board,bool **visibility,int **solutions,int size,int hints,int hidden,int hidden_limit);
SudokuBoard* Sudokuboard_init(int size,int hints);

#endif 
