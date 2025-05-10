#include "board.h"
#include "utility.h"
#include "autosave.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "solver.h"

int main() {
    srand(time(NULL));

    SudokuBoard *sudoku = NULL;
    
    bool board_exists = false;
    bool is_solved = false;

    create_autosave_file();
    sudoku = load_sudoku();
    
    // Check if board is loaded
    if (sudoku->size == 4 || sudoku->size == 9 || sudoku->size == 16) {
        system("clear");
        printf("Board loaded successfully.\n");
        sleep(1);
        system("clear");
        board_exists = true;
    }
        
    while(1){
        
        system("clear");
        printf("   _____  _    _  _____   ____   _  __ _  _ \n");
        printf("  / ____|| |  | ||  __ \\ |  _ \\ | |/ /| || |\n");
        printf(" | (___  | |  | || |  | || | | || ' / | || |\n");
        printf("  \\___ \\ | |  | || |  | || | | ||  <  | || |\n");
        printf("  ____) || |__| || |__| || |_| || . \\ | || |\n");
        printf(" |_____/  \\____/ |_____/ |____/ |_|\\_\\|____|\n");
        printf("\n        SUDOKU GAME by pfczx\n\n");
        printf("\n");
        printf("MENU:\n");
        printf("1. Play\n");
        printf("2. Generate/Regenerate Board\n");
        printf("3. Game Info\n");
        printf("4. Solver\n");
        printf("5. Exit\n");
        printf("\n");
        printf("Enter your choice: ");

        int choice;
        int size;
        int hints;

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (!board_exists) {
                    printf("Please generate a board first.\n");
                    sleep(1);
                    system("clear");
                    clear_buffer();
                    break;
                }

                time_t start_t=time(NULL);
                int moves_count=0;
                //loop for playing
                while (1) {
                    clear_buffer();
                    system("clear");
                    if (is_solved) {
                        printf("Congratulations! You solved the Sudoku!\n");
                    }
                    else {
                        printf("Current board not solved yet.\n");
                    }

                    time_t current_t=time(NULL);
                    int elapsed_time = (int)difftime(current_t, start_t);
                    int minutes = elapsed_time / 60;
                    printf("Elapsed time: %d minutes and %d seconds\n", minutes, elapsed_time % 60);
                    printf("Moves count: %d\n", moves_count);

                    //move
                    int row, col, num;
                    printf("\n");
                    print_board(sudoku->board, sudoku->size);
                    printf("\n");
                    printf("Select row,col and number, 0 0 0 for exit\n");
                    printf("Select row: ");
                    scanf("%d",&row);
                    printf("Select col: ");
                    scanf("%d",&col);
                    printf("Select number: ");
                    scanf("%d",&num);
                    printf("\n");
                    
                   
                    if (row == 0 && col == 0 && num == 0) {
                        break;
                    }
                    // Adjusting for 0-based indexing
                    row--;
                    col--;
                    if (check_move_inputs(row, col, num, sudoku->board, sudoku->visibility, sudoku->size)) {
                        sudoku->board[row][col] = num;
                        is_solved = check_sudoku_state(sudoku->board, sudoku->visibility, sudoku->solutions, sudoku->size);
                        moves_count++;
                        save_sudoku(sudoku);
                    }
                    else {
                        printf("Invalid move. Please try again.\n");
                        sleep(1);
                        
                    }
                    
                    
                }
                system("clear");
                break;
            case 2:
                //clearning previous board
                if (board_exists) {
                    system("clear");
                    printf("Clearnig previous board.\n");
                    clear_sudoku(sudoku);
                    sudoku = NULL;
                    sleep(1);
                    printf("Done\n");
                    
                             
                }
                system("clear");

                do{
                    clear_buffer();
                    printf("Enter board size (4,9,16): ");
                    scanf("%d", &size);
                    printf("Enter number of hints (minimal ammount 24 for 9x9, 125 for 16x16): ");
                    scanf("%d", &hints);
                    system("clear");


                }while (!check_board_inputs(size, hints));

                system("clear");
                printf("Generating board...\n");
                sleep(1);
                system("clear");
                sudoku = Sudokuboard_init(size, hints);
                clear_buffer();
                board_exists = true;
                is_solved = false;
                printf("Board generated successfully, go to play and GOOD LUCK! :).\n");
                print_board(sudoku->board, sudoku->size);
                sleep(2);
                system("clear");
                break;
            case 3:
                system("clear");
                printf("Game Info:\n");
                printf("--This is a simple Sudoku game.\n");                 
                printf("--Your current board will be autosaved after every move and load when starting script if you wish to come back :).\n");
                printf("--Save file will be located in the game dir (autosave.txt), do not rename it.\n");
                printf("--You can generate a new board at any time.\n");
                printf("--Erors will be shown if you try to make an invalid move but try to avoid pressing ENTER too many times.\n");
                printf("--The game is created by pfczx.\n");
                printf("Press enter to back to menu\n");
                clear_buffer();
                getchar();
                break;
            case 4:
                system("clear");
                if (!board_exists) {
                    printf("Please generate a board first.\n");
                    sleep(1);
                    system("clear");
                    clear_buffer();
                    break;
                }
                printf("Starting solver...\n");
                solver(sudoku);
                system("clear");
            break;
            case 5:
                exit(0);
                break;

              
                
            default:
                system("clear");
                printf("Wrong input ;(.\n");
                sleep(1);
                system("clear");
                clear_buffer();
                break;
                
        }

    }
    return 0;
}
