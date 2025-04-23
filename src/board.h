#ifndef BOARD_H
#define BOARD_H
#include <stdbool.h>
#include <time.h>

typedef struct {
    int **board;
    bool **visibility;
    int  **inputs;
 } SudokuBoard;

bool check_new_number(int **board, int row, int col, int num, int size);
void fill_board(int **board,bool **visibility, int size,int row, int col,time_t time);
void set_visibility(bool **visibility, int size,int hints);
bool is_one_solution(int **board,bool **visibility,int size,int hints,time_t time);
SudokuBoard* Sudokuboard_init(int size,int hints);

#endif 
