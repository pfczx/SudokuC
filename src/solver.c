#include "solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "board.h"

bool check_number(int **board, int row, int col, int num, int size) {
    // Check the row
    for (int j = 0; j < size; j++) {
        if (board[row][j] == num && j != col) {
            return false;
        }
    }

    // Check the column
    for (int i = 0; i < size; i++) {
        if (board[i][col] == num && i != row) {
            return false;
        }
    }

    // Check the grid
    int grid_size = sqrt(size);
    int start_row = row - row % grid_size;
    int start_col = col - col % grid_size;
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            if (board[i + start_row][j + start_col] == num && (i + start_row != row || j + start_col != col)) {
                return false;
            }
        }
    }

    return true;
}
void init_for_solver(SudokuBoard *sudoku) {

    for(int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            if (sudoku->visibility[i][j] == 0) {
                sudoku->board[i][j] = 0; 
                }

        }
    }

    int grid_size = sqrt(sudoku->size);
    
    //filling board random numbers with grid checking
    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {

            while(sudoku->board[i][j]==0){

                int value = (rand() % sudoku->size) + 1;
                bool is_value_used = false;

                int start_row = i - i % grid_size;
                int start_col = j - j % grid_size;
                for (int h = 0; h < grid_size; h++) {
                    for (int g = 0; g < grid_size; g++) {
                        if (sudoku->board[h + start_row][g + start_col] == value) {
                            is_value_used = true;
                        }
                    }
                }
                if (!is_value_used){
                    sudoku->board[i][j]=value;
                }
                
            }
        }
    }
}

int calculate_cost(SudokuBoard *sudoku){
    int cost = 0;
    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            if (sudoku->visibility[i][j] == false) {
                if (!check_number(sudoku->board, i, j, sudoku->board[i][j], sudoku->size)) {
                    cost++;
                }
            }
        }
    }
    return cost;
}

bool generate_neighbour(SudokuBoard *sudoku, int *row1, int *col1, int *row2, int *col2) { 
    int grid_size = sqrt(sudoku->size);
    int attempts = 0;
    int max_attempts = 10;
    while (attempts <max_attempts){

    //selecting grid
    int grid_row = rand() % grid_size;
    int grid_col = rand() % grid_size;
    int start_row = grid_row * grid_size;
    int start_col = grid_col * grid_size;

    //selecting two random cells in the grid
    do {
        *row1 = start_row + rand() % grid_size;
        *col1 = start_col + rand() % grid_size;
        attempts++;
        if (attempts >= max_attempts) {
            return false;
        }
    } while (sudoku->visibility[*row1][*col1]); 

    attempts = 0;
    do {
        *row2 = start_row + rand() % grid_size;
        *col2 = start_col + rand() % grid_size;
        attempts++;
        if (attempts >= max_attempts) {
            return false;
        }
    } while (sudoku->visibility[*row2][*col2] || (*row1 == *row2 && *col1 == *col2));

    
    int temp = sudoku->board[*row1][*col1];
    sudoku->board[*row1][*col1] = sudoku->board[*row2][*col2];
    sudoku->board[*row2][*col2] = temp;
    return true;
    

}
return false;
}
void revert_swap(SudokuBoard *sudoku, int row1, int col1, int row2, int col2) {
    int temp = sudoku->board[row1][col1];
    sudoku->board[row1][col1] = sudoku->board[row2][col2];
    sudoku->board[row2][col2] = temp;
}

void solver(SudokuBoard *sudoku){
    init_for_solver(sudoku);
    int cost = calculate_cost(sudoku);
    int new_cost;

    double temperature = 1000.0;
    double cooling_rate = 0.99995;

    int row1, col1, row2, col2;


    while (1)
    {
        
        if (!generate_neighbour(sudoku, &row1, &col1, &row2, &col2)){
            continue;
        }

        new_cost = calculate_cost(sudoku);
        int delta_cost = new_cost - cost;

        if(new_cost < cost) {
            // Accept the new state
            cost = new_cost;
        } else {
            // Calculate acceptance probability
            double acceptance_probability = exp(-delta_cost / temperature);
            if ((double)rand() / RAND_MAX < acceptance_probability) {
                // Accept the new state
                cost = new_cost;
            } else {
                // Revert the swap
                revert_swap(sudoku, row1, col1, row2, col2);
            }
        }
        if (temperature < 0.001){
            break;
        }

        if (cost == 0) {
            break;
        }
        temperature *= cooling_rate;
        printf("Current cost: %d, Temperature: %.5f\n", cost, temperature);
        
        
    }
    printf("Finished with cost: %d\n", cost);
    sleep(1);
    

    
}      