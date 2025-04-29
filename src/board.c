#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

        
// setting visibility of cells randomly  ensuring that the board has only one solution
void set_one_solution(int **board,bool **visibility,int **solutions,int size,int hints,int hidden,int hidden_limit){
    //end of recursion
    if (hidden == hidden_limit) {
        return;             
    }
    //looking for a random cell to hide
    while(true){
    int row = rand() % size;
    int col = rand() % size;
    if (visibility[row][col] == false) {
        continue;
    }

    int removing_number = board[row][col];
    board[row][col] = 0;
    visibility[row][col] = false;
    solutions[row][col] = removing_number;


    //checking number of solutions(max 2)
    int count = 0;
    count_solutions(board,visibility,solutions,size,0,0,&count,2);
    //solutions number stored in &count
    //1 solution continue, else backtrack
    if (count == 1){
        set_one_solution(board,visibility,solutions,size,hints,hidden + 1,hidden_limit);
        return;
    } 
    else {
        board[row][col] = removing_number;
        visibility[row][col] = true;
        solutions[row][col] = 0;
        
    
    }
}
 
}
void count_solutions(int **board,bool **visibility,int **solutions,int size,int row,int col,int *solutions_counter,int solution_limit) {
    //end of recursion
    if (*solutions_counter > solution_limit) {
        return;
    }
    if (col == size) {
        count_solutions(board,visibility,solutions,size,row + 1, 0,solutions_counter,solution_limit);
        return;
    }
    //full board, solution found
    if (row == size) {
        (*solutions_counter)++;
        return;
    }
    //skipping cells that are visible
    if (visibility[row][col] == true) {
        count_solutions(board,visibility,solutions,size,row, col + 1,solutions_counter,solution_limit);
        return;
    }
    //trying to fill the cell with numbers from 1 to size and check all possibilities
    for (int num = 1; num <= size; num++) {
        if (check_new_number(board, row, col, num, size)) {
            board[row][col] = num;
            count_solutions(board,visibility,solutions,size,row, col + 1,solutions_counter,solution_limit);
            //backtracking
            board[row][col] = 0;
        }
    }
      
}

//checking vertical, horizontal and subgrid sqrt(size) coz of sudoku rules
bool check_new_number(int **board, int row, int col, int num, int size) {
 
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
                board[i][j] = 0;
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
    //scaling attempts to size
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

SudokuBoard* Sudokuboard_init(int size, int hints)  {

    bool **visibility = (bool **) malloc(size * sizeof(bool *));
    for (int i = 0; i < size; i++) {
        visibility[i] = (bool *) malloc(size * sizeof(bool));
    }
    //setting visibility of cells
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            visibility[i][j] = true;
        }
    }

    int **board = (int **) malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        board[i] = (int *) malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            board[i][j] = 0;
        }
    }

    int **solutions = (int **) malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        solutions[i] = (int *) malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            solutions[i][j] = 0;
        }
    }


    fill_board(board,visibility, size,0,0,time(NULL));

    int hidden_limit = size * size - hints;
    set_one_solution(board,visibility,solutions,size,hints,0,hidden_limit);
    
    SudokuBoard *sudoku_board = (SudokuBoard *) malloc(sizeof(SudokuBoard));

    sudoku_board->size = size;
    sudoku_board->board = board;
    sudoku_board->visibility = visibility;
    sudoku_board->solutions = solutions; 

    return sudoku_board;


}
