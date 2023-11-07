# sudoku
## Exercise in getting back into coding
### sudoku.c
This is the final result of the exercise.
The program creates a linked list containing the row, column, and value for each unknown value.
It iterates through each of unknown and checks every value if it is valid for that location on the board.
If all values are invalid for a particular location, it means that a previous element in the linked list is incorrect.
In these instances the program goes to the previous linked list element until it finds the next viable value in that location.
### recSudoku.c
This was created with the intention to do a proof of concept of solving sudoku puzzles, but utilizes a lot of memory as it creates a new 2D array for every unknown value.
