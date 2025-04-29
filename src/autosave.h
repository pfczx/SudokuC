#ifndef AUTOSAVE_H
#define AUTOSAVE_H
#include "board.h"

void create_autosave_file();
void save_sudoku(SudokuBoard *sudoku);
SudokuBoard* load_sudoku();


#endif