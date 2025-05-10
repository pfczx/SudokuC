#ifndef SOLVER_H
#define SOLVER_H
#include "board.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


bool check_number(int **board, int row, int col, int num, int size);
//init struct with filled board randomly from solutions
void init_for_solver(SudokuBoard *sudoku);
//calculate cost of the board
int calculate_cost(SudokuBoard *sudoku);
//swith 2 numbers in the board
bool generate_neighbour(SudokuBoard *sudoku, int *row1, int *col1, int *row2, int *col2);
//revert
void revert_swap(SudokuBoard *sudoku, int row1, int col1, int row2, int col2);
void solver(SudokuBoard *sudoku);

#endif