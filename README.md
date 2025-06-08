# 🧩 SudokuC

**SudokuC** is a terminal-based Sudoku game written in C, designed for a simple puzzle-solving. It supports different board sizes, autosaves progress after every move.

---

## ✨ Features

- ✅ Play Sudoku in your terminal with a clean menu-driven interface.
- 🔄 Auto-saves game progress to a local file (`autosave.txt`) after each move.
- 💡 Custom board generation with selectable size and number of hints:
  - Supported sizes: **4x4**, **9x9**, and **16x16**.
- 🔍 Move validation with real-time win condition checks.
- ♻️ Regenerate the board at any time.
- 🧪 Built-in input validation to prevent invalid entries.
- 💻 Genetic algorithm and Simulated Annealing for solving sudoku for you.

---

## 🧠 Board Generation: Recursive Backtracking

The Sudoku board is generated using a **recursive backtracking algorithm**, a common technique in constraint satisfaction problems like Sudoku.

Here's how it works:
1. The algorithm fills the board one cell at a time.
2. For each cell, it attempts to place a number that doesn't violate Sudoku rules.
3. If no valid number can be placed, it backtracks to the previous cell and tries a different value.
4. This continues recursively until the entire board is filled with a valid solution.
5. Then another backtracking algorithm will be removing cells one by one ensuring sudoku has only one solution

---

https://github.com/user-attachments/assets/0e3058f8-e26a-4be4-9826-1930a2542ca5

---

## 🔨 Building 
src
```bash
gcc -c main.c -o ../build/main.o
gcc -c board.c -o ../build/board.o
gcc -c utility.c -o ../build/utility.o
gcc -c autosave.c -o ../build/autosave.o
gcc -c solver.c -o ../build/autosave.o
gcc -c genetic_solver.c -o ../build/autosave.o

```
build
```bash
gcc main.o board.o utility.o autosave.o solver.o genetic_solver.o -o sudoku -lm 
```
---
![ac](https://github.com/user-attachments/assets/4a2dbf26-73a5-4808-a13b-aaf432c1580a)


