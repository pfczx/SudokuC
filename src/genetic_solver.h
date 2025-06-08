#ifndef GENETIC_SOLVER_H
#define GENETIC_SOLVER_H

#include "board.h"
#include "solver.h"

typedef struct {
    SudokuBoard *sudoku;
    int cost;
} Individual;

void create_population(SudokuBoard *sudoku, Individual *population);
void sort_population(Individual *population);
void mutate(Individual *individual);
void copy_board(SudokuBoard *dest, SudokuBoard *src);
void genetic_solver(SudokuBoard *sudoku);

#endif 


