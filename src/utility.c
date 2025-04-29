#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n');
}

void print_board(int **board, int size) {

    int subgrid_size = (int)sqrt(size); 
    int line_length;
    switch(size) {
        case 4:
            line_length = 11;
            break;
        case 9:
            line_length = 20;
            break;
        case 16:
            line_length = 32;
            break;
        default:
            line_length = 0;
    }
    //col index
    printf("       ");
    for (int i = 0; i < size; i++) {
        printf("%2d", i + 1);
        if (i != 0 && (i+1) % subgrid_size == 0) {
            printf("    ");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    for (int i = 0; i < line_length; i++) {
        printf("--");
    }
    printf("\n");
                     
    for (int i = 0; i < size; i++) {
        //row index
        printf("%2d", i + 1);
        printf("   | ");
        for (int j = 0; j < size; j++) {
            if (j != 0 && j % subgrid_size == 0) {
                printf(" | ");
                
            }

            printf("%2d ", board[i][j]);
        }

        printf("\n");

        
        if ((i + 1) % subgrid_size == 0 && i != size - 1) {
            for (int j = 0; j < line_length; j++) {
                printf("--");
            }
            printf("\n");
        }
    }
}

bool check_board_inputs(int size, int hints) {
    if (size != 4 && size != 9 && size != 16) {
        printf("\nInvalid board size\n");
        return false;
    }
    if (hints < 0 || hints > size * size) {
        printf("\nInvalid number of hints. Please enter a reasonable value\n");
        return false;
    }
    return true;
}
void clear_sudoku(SudokuBoard *sudoku_board) {
    if (sudoku_board == NULL) {
        return;
    }
    for (int i = 0; i < sudoku_board->size; i++) {
        free(sudoku_board->board[i]);
        free(sudoku_board->visibility[i]);
        free(sudoku_board->solutions[i]);
    }
    free(sudoku_board->board);
    free(sudoku_board->visibility);
    free(sudoku_board->solutions);
    free(sudoku_board);
}
bool check_move_inputs(int row, int col, int num, int **board, bool **visibility, int size) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        printf("\nInvalid cell coordinates.\n");
        return false;
    }
    if (num < 1 || num > size) {
        printf("\nInvalid number. Please enter a number between 1 and %d.\n", size);
        return false;
    }
    if (visibility[row][col]) {
        printf("\nCell is not visible. You cannot change its value.\n");
        return false;
    }
    return true;
}
bool check_sudoku_state(int **board, bool **visibility, int **solutions, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (visibility[i][j] == false) {
                if (board[i][j] != solutions[i][j]) {
                    return false; 
                    
                }
            }
        }
    }
    return true;
}