#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>     
#include "board.h"           

bool check_board_inputs(int size,int hints);
void clear_sudoku(SudokuBoard *sudoku_board);
bool check_move_inputs(int row,int col,int num,int **board,bool **visibility,int size);
void print_board(int **board, int size);
bool check_sudoku_state(int **board, bool **visibility, int **inputs, int size);
void clear_buffer();

#endif