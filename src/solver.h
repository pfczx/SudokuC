#ifndef SOLVER_H
#define SOLVER_H
#include "board.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


bool check_number(int **board, int row, int col, int num, int size);
void init_for_solver(SudokuBoard *sudoku);
int calculate_cost(SudokuBoard *sudoku);
bool generate_neighbour(SudokuBoard *sudoku, int *row1, int *col1, int *row2, int *col2);
void revert_swap(SudokuBoard *sudoku, int row1, int col1, int row2, int col2);
void solver(SudokuBoard *sudoku);

#endif