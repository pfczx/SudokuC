#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>


static volatile sig_atomic_t timed_out = 0;

void timeout_handler(int sig) {
    timed_out = 1;
}

// checking by 50x randomly filled board if all solution numbers(nonvisible cells) are the same
bool is_one_solution(int **board, bool **visibility, int size, time_t started_t) {
    
    int attempts = 3000;
    int **temp_board = (int **) malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        temp_board[i] = (int *) malloc(size * sizeof(int));
    }

    int **solutions = (int **) malloc(attempts * sizeof(int *));
    for (int i = 0; i < attempts; i++) {
        solutions[i] = (int *) malloc((size *size) * sizeof(int));
    }

    for (int loop = 0; loop < attempts; loop++) {
        
     
        //reset temp_board to original board  
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (visibility[i][j] == false) {
                    temp_board[i][j] = 0;
                } else {
                    temp_board[i][j] = board[i][j];
                }
                
            }
        }
        //if recursion takes too long brak function
        signal(SIGALRM, timeout_handler);
        alarm(5);
        fill_board(temp_board,visibility, size, 0, 0,time(NULL));
        alarm(0);
        
        //copying nonvisibles as sollutioons
        int solutions_counter=0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (visibility[i][j] == false) {
                    solutions[loop][solutions_counter] = temp_board[i][j];
                    solutions_counter++;
                } 
            }
        }
        
    }
    //checking if all solutions are the same
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < attempts; j++) {
            if (solutions[0][i] != solutions[j][i]) {
                return false;
            }
        }
    }
    
    return true;
     
    

}
//checking vertical, horizontal and subgrid sqrt(size) coz of sudoku rules
bool check_new_number(int **board, int row, int col, int num, int size) {
    //alarm needed for is_one_solution function 
    if (timed_out) {
        return false;
    }
    
    for (int i = 0; i < size; i++) {
        if (board[row][i] == num) {
            return false;
        }
    }

    
    for (int i = 0; i < size; i++) {
        if (board[i][col] == num) {
            return false;
        }
    }  

    int grid_size = sqrt(size);
    int start_row = row - row % grid_size;
    int start_col = col - col % grid_size;
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            if (board[i + start_row][j + start_col] == num) {
                return false;
            }
        }
    }

    return true;
}
//filling the board with random numbers(1-size), recursive,reset if time is up(0.2s) 
void fill_board(int **board,bool **visibility, int size, int row, int col,time_t started_t) {
    
    //reset if time is up
    if (difftime(time(NULL),started_t) > 0.2) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                //condition needed to reset only nonvisible cells(all are nonvisible when creating board), reusing pourposes
                if (visibility[i][j] == false) {
                board[i][j] = 0;
                }
                
            }        
        }

        fill_board(board,visibility, size, 0, 0,time(NULL));
    }
    //next row
    if (col == size) {
        fill_board(board,visibility, size, row + 1, 0,started_t);
        return;
    }
    // whole board complete
    if (row == size) {
        return;
    }                     
    int attempts = 0;
    // condition for reusing func in is_one_solution
    if (board[row][col] != 0) {
        fill_board(board,visibility, size, row, col + 1,started_t);
        return;
    }
    while (attempts < size) {
        int num = (rand() % size) + 1;
        attempts++;

        if (check_new_number(board, row, col, num, size)) {
            board[row][col] = num;
            fill_board(board,visibility, size, row, col + 1,started_t);
                       
            if (board[size - 1][size - 1] != 0 ){
                    return;
                }

            
        }
        //backtracking
        board[row][col] = 0;
    }
}



//setting visibility of cells randomly
void set_visibility(bool **visibility, int size, int hints) {


    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            visibility[i][j] = false;
        }
    }

    
    int visible_count = 0;
    while (visible_count < hints) {
        int random_row = rand() % size;
        int random_col = rand() % size;

        
        if (visibility[random_row][random_col] == true) {
            continue;
        }

        
        visibility[random_row][random_col] = true;
        visible_count++;
    }
}

SudokuBoard* Sudokuboard_init(int size, int hints)  {
    bool **visibility = (bool **) malloc(size * sizeof(bool *));
    for (int i = 0; i < size; i++) {
        visibility[i] = (bool *) malloc(size * sizeof(bool));
    }

    int **board = (int **) malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        board[i] = (int *) malloc(size * sizeof(int));
    }

    int **inputs = (int **) malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        inputs[i] = (int *) malloc(size * sizeof(int));
    }


    fill_board(board,visibility, size,0,0,time(NULL));

    do {
        set_visibility(visibility, size, hints);
        

    }while (!is_one_solution(board,visibility,size,time(NULL)));

    
    
    
                      
    SudokuBoard *sudoku_board = (SudokuBoard *) malloc(sizeof(SudokuBoard));
    sudoku_board->board = board;
    sudoku_board->visibility = visibility;
    sudoku_board->inputs = inputs;
    return sudoku_board;


}
