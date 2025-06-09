#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "genetic_solver.h"
#include "board.h"

#define POP_SIZE 5000
#define MUTATION_RATE 0.4
#define MAX_GENERATIONS 1000
#define STAGNATION_LIMIT 10

void free_sudoku(SudokuBoard *sudoku) {
    for (int i = 0; i < sudoku->size; i++) {
        free(sudoku->board[i]);
        free(sudoku->visibility[i]);
    }
    free(sudoku->board);
    free(sudoku->visibility);
    free(sudoku);
}

void create_population(SudokuBoard *sudoku, Individual *population) {
//filling original sudoku board with random numbers
    init_for_solver(sudoku);
//creating population based on the original sudoku board
    for (int i = 0; i < POP_SIZE; i++) {
        population[i].sudoku = (SudokuBoard *)malloc(sizeof(SudokuBoard));
        population[i].sudoku->size = sudoku->size;
        population[i].sudoku->board = (int **)malloc(sudoku->size * sizeof(int *));
        population[i].sudoku->visibility = (bool **)malloc(sudoku->size * sizeof(bool *));
        for (int j = 0; j < sudoku->size; j++) {
            population[i].sudoku->board[j] = (int *)malloc(sudoku->size * sizeof(int));
            population[i].sudoku->visibility[j] = (bool *)malloc(sudoku->size * sizeof(bool));
            for (int k = 0; k < sudoku->size; k++) {
                population[i].sudoku->board[j][k] = sudoku->board[j][k];
                population[i].sudoku->visibility[j][k] = sudoku->visibility[j][k];
            }
        }
    }
// generating neighbours for each individual in the population
    for (int i = 0; i < POP_SIZE; i++) {
        int row1, col1, row2, col2;
        for (int j = 0; j < sudoku->size*sudoku->size; j++) { 
            generate_neighbour(population[i].sudoku, &row1, &col1, &row2, &col2);
        }
        population[i].cost = calculate_cost(population[i].sudoku);
    }
}

void sort_population(Individual *population) {
    for (int i = 0; i < POP_SIZE - 1; i++) {
        for (int j = i + 1; j < POP_SIZE; j++) {
            if (population[i].cost > population[j].cost) {
                Individual temp = population[i];
                population[i] = population[j];
                population[j] = temp;
            }
        }
    }
}

void mutate(Individual *individual) {
    int row1, col1, row2, col2;
    if (generate_neighbour(individual->sudoku, &row1, &col1, &row2, &col2)) {
        individual->cost = calculate_cost(individual->sudoku); 
    }
}

void copy_board(SudokuBoard *child, SudokuBoard *parent) {
    child->size = parent->size;
    for (int i = 0; i < parent->size; i++) {
        for (int j = 0; j < parent->size; j++) {
            child->board[i][j] = parent->board[i][j];
            child->visibility[i][j] = parent->visibility[i][j];
        }
    }
}
//crossover function that creates a child individual from two parents
void crossover(Individual *parent1, Individual *parent2, Individual *child) {
    int size = parent1->sudoku->size;

    child->sudoku = (SudokuBoard *)malloc(sizeof(SudokuBoard));
    child->sudoku->size = size;
    child->sudoku->board = (int **)malloc(size * sizeof(int *));
    child->sudoku->visibility = (bool **)malloc(size * sizeof(bool *));
    for (int i = 0; i < size; i++) {
        child->sudoku->board[i] = (int *)malloc(size * sizeof(int));
        child->sudoku->visibility[i] = (bool *)malloc(size * sizeof(bool));
    }

    copy_board(child->sudoku, parent1->sudoku);

    
    for (int i = 0; i < size; i++) {
        if (rand() % 2 == 0) {
            for (int j = 0; j < size; j++) {
                if (!child->sudoku->visibility[i][j]) {
                    child->sudoku->board[i][j] = parent2->sudoku->board[i][j];
                }
            }
        }
    }

    child->cost = calculate_cost(child->sudoku);
}



void genetic_solver(SudokuBoard *sudoku) {

    int stagnation_count = 0;
    

    Individual population[POP_SIZE];
    create_population(sudoku, population);

    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        sort_population(population);

        if (population[0].cost == 0) {
            printf("Solution found in generation %d\n", generation);
            copy_board(sudoku, population[0].sudoku);
            
            
            for (int i = 0; i < POP_SIZE; i++) {
                free_sudoku(population[i].sudoku);
            }
            return;
        }

        //reset worse half population if stagnation occurs, mutate better half
        if (generation > 0 && population[0].cost == population[1].cost) {
            stagnation_count++;
            if (stagnation_count >= STAGNATION_LIMIT) {

                for (int i = POP_SIZE /2; i < POP_SIZE; i++) {
                    init_for_solver(population[i].sudoku); 
                    int row1, col1, row2, col2;
                    generate_neighbour(population[i-POP_SIZE/2].sudoku, &row1, &col1, &row2, &col2);
                    population[i].cost = calculate_cost(population[i].sudoku);
                    
                }
                
            }
        } else {
            stagnation_count = 0;
        }

        Individual new_population[POP_SIZE];
        for (int i = 0; i < POP_SIZE / 2; i++) {
            crossover(&population[i], &population[i + 1], &new_population[i * 2]);

            //  cloning
            new_population[i * 2 + 1].sudoku = (SudokuBoard *)malloc(sizeof(SudokuBoard));
            int size = population[i + 1].sudoku->size;
            new_population[i * 2 + 1].sudoku->size = size;
            new_population[i * 2 + 1].sudoku->board = (int **)malloc(size * sizeof(int *));
            new_population[i * 2 + 1].sudoku->visibility = (bool **)malloc(size * sizeof(bool *));
            for (int j = 0; j < size; j++) {
                new_population[i * 2 + 1].sudoku->board[j] = (int *)malloc(size * sizeof(int));
                new_population[i * 2 + 1].sudoku->visibility[j] = (bool *)malloc(size * sizeof(bool));
                for (int k = 0; k < size; k++) {
                    new_population[i * 2 + 1].sudoku->board[j][k] = population[i + 1].sudoku->board[j][k];
                    new_population[i * 2 + 1].sudoku->visibility[j][k] = population[i + 1].sudoku->visibility[j][k];
                }
            }
            new_population[i * 2 + 1].cost = calculate_cost(new_population[i * 2 + 1].sudoku);

            // mutation
            if ((double)rand() / RAND_MAX < MUTATION_RATE) {
                mutate(&new_population[i * 2]);
            }
            //addidional mutatuion if cost is low
            if (sudoku->size < population[i].cost) {
                mutate(&new_population[i * 2 + 1]);
            }
        }

        //switch population
        for (int i = 0; i < POP_SIZE; i++) {
            free_sudoku(population[i].sudoku);
        }

        for (int i = 0; i < POP_SIZE; i++) {
            population[i] = new_population[i];
        }
        printf("Generation %d: Best cost = %d\n", generation, population[0].cost);
    }

    printf("No solution found within the maximum generations, cost = %d.\n", population[0].cost);

    for (int i = 0; i < POP_SIZE; i++) {
        free_sudoku(population[i].sudoku);
    }
}
