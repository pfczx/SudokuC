#include "autosave.h"
#include "stdio.h"
#include "stdbool.h"
#include <unistd.h>
#include "stdlib.h"



void create_autosave_file() {
    // check if alrdeady exists
    if (fopen("autosave.txt", "r") == NULL) {
        FILE *file = fopen("autosave.txt", "w");
        if (file == NULL) {
            printf("Error creating autosave file.\n");
            printf("Please check script permissions.\n");
            exit(1);
            return;
        }
        fclose(file);
    }
}            

void save_sudoku(SudokuBoard *sudoku) {
    FILE *file = fopen("autosave.txt", "w");
    if (file == NULL) {
        printf("Error opening autosave file.\n");
        printf("Please check script permissions.\n");
        exit(1);
        return;
    }
    fprintf(file, "%d\n", sudoku->size);
    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            fprintf(file, "%d ", sudoku->board[i][j]);
        }
        fprintf(file, "\n");
    }
    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            fprintf(file, "%d ", sudoku->visibility[i][j]);
        }
        fprintf(file, "\n");
    }
    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            fprintf(file, "%d ", sudoku->solutions[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

SudokuBoard* load_sudoku() {
    FILE *file = fopen("autosave.txt", "r");
   
    SudokuBoard *sudoku = (SudokuBoard *) malloc(sizeof(SudokuBoard));

    fscanf(file, "%d", &sudoku->size);

    sudoku->board = (int **) malloc(sudoku->size * sizeof(int *));
    for (int i = 0; i < sudoku->size; i++) {
        sudoku->board[i] = (int *) malloc(sudoku->size * sizeof(int));
    }
    sudoku->visibility = (bool **) malloc(sudoku->size * sizeof(bool *));
    for (int i = 0; i < sudoku->size; i++) {
        sudoku->visibility[i] = (bool *) malloc(sudoku->size * sizeof(bool));
    }
    sudoku->solutions = (int **) malloc(sudoku->size * sizeof(int *));
    for (int i = 0; i < sudoku->size; i++) {
        sudoku->solutions[i] = (int *) malloc(sudoku->size * sizeof(int));
        
    }

    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            fscanf(file, "%d", &sudoku->board[i][j]);
        }
    }
    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            fscanf(file, "%d", &sudoku->visibility[i][j]);
        }
    }
    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            fscanf(file, "%d", &sudoku->solutions[i][j]);
        }
    }
    fclose(file);
    return sudoku;


}