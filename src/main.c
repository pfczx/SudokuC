#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Function to print the Sudoku board
void print_board(int **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
}
//function to print visibility
void print_visibility(bool **visibility,int size){
for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
        printf("%2d ", visibility[i][j]);
    }
    printf("\n");
}
}

void print_complete_board(int **board, bool **visibility, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (visibility[i][j]) {
                printf("%2d ", board[i][j]);
            } else {
                printf(" 0 ");
            }
        }
        printf("\n");
    }
}
int main() {
    srand(time(NULL)); 

    printf("Sudoku Generator main odpalony\n");
    int size = 16;
    // for 16 - 80
    // for 9 - 20  
    int hints = 60; 

    SudokuBoard *sudoku = Sudokuboard_init(size, hints);

    
    print_board(sudoku->board, size);
    printf("-----------------------\n");
    print_visibility(sudoku->visibility,size);
    printf("-----------------------\n");
    
    print_complete_board(sudoku->board, sudoku->visibility, size);

    return 0;
}