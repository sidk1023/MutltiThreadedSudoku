# MutltiThreadedSudoku
A multithreaded sudoku solver in C.

This code is a part of an Operating Systems assignment on parallel programming in linux at BITS Pilani Hyd Campus

Uses pthread to solve an NxN sudoku (N = {9,16,25,36})

Input: .txt file with unsolved Sudoku

Output: Solved sudoku printed in terminal

Usage: 
1. Compile: gcc -o solve sudoku.c -lpthread -lm
2. Run: ./solve N <filepath to NxN sudoku .txt file>
  
Example txt file is provided
