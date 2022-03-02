# MutltiThreadedSudoku
A multithreaded sudoku solver in C.

Uses pthread to solve an NxN sudoku (N = {9,16,25,36})

Input: .txt file with unsolved Sudoku

Output: Solved sudoku printed in terminal

Usage: 
1. Compile: gcc -o solve sudoku.c -lpthread -lm
2. Run: ./solve <N> <filepath to NxN sudoku .txt file>
  
Example txt file is provided
