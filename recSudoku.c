#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BOARDSIZE 9
#define BOARDSQR (int)sqrt(BOARDSIZE)

/* ##### READ ME #####
 * This code utilizes the term *square* to refer to the NxN squares present inside the puzzle.
 * For example, if the sudoku puzzle was a traditional 9x9 puzzle, there would be 9 3x3 squares
 * within the puzzle, like the two below.
 * _______ _______
 * |1|2|3| |4|5|6|
 * |4|5|6| |7|8|9|
 * |7|8|9| |1|2|3|
 * _______ _______
 */

// ##### STRUCT DECLARATIONS #####

// Declare node to create linked list of possible values for each number in the puzzle
typedef struct node {
	// Data stored in current node
	int val;

	//link to next node
	struct node * next;	
}	node_val;

// ##### FUNCTION DECLARATIONS #####

// Solve Board
void solveBoard( short int puzzle[BOARDSIZE][BOARDSIZE], int row, int col );

// Print board
void printBoard( short int puzzle[BOARDSIZE][BOARDSIZE] );

// ##### MAIN FUNCTION #####
int main(int argc, char *argv[]){

	//Tell user the current size of the board.
	//This may change to a dynamic user input, but I do not feel like bothering with it currently.
	printf("The current size of the board is %dx%d.\n", BOARDSIZE, BOARDSIZE);

	// Validates that user inputs are within the bounds of the board size
	int temp = 1;
	while( temp < argc ){
		if( !(atoi( argv[temp] ) > 0 && atoi( argv[temp] ) <= BOARDSIZE) ){

			// Tells the user that they gave and invalid input and exits the program
			printf( "Please only input values within the designated size of the board.\n" );
			return 0;
		}
		temp += 1;
	}
		
	//Create a time value for the random number generator
	time_t t;
	
	//set a random seed for random number generation
	srand((unsigned) time(&t));

	// Initialize blank puzzle
	short int puzzle[BOARDSIZE][BOARDSIZE];
	for( int i = 0; i < BOARDSIZE; i++ ){
		for( int j = 0; j < BOARDSIZE; j++ ){
			puzzle[i][j] = 0;
		}
	}

	// Takes user inputs and places them in row column order from bottom left to top right
	// Counting starts at 1, 1 for bottom left corner of board
	// This can be changed to a loop of scanfs in the future to enable more user interaction, but I do not feel like bothering with it currently.
	for( int i = 1; i < argc - 1; i+=3 ){
		puzzle[BOARDSIZE - atoi(argv[i+1])][atoi(argv[i])-1] = (short int) atoi(argv[i + 2]);
	}

	//Label unsolved puzzle
	printf("UNSOLVED PUZZLE!\n");

	//Print unsolved puzzle
	printBoard(puzzle);
	
	//Seperate space and label solved puzzle
	printf("\nSOLVED PUZZLE!\n");

	// Solve puzzle
	solveBoard(puzzle, 0, 0);

	return 0;
}

// ##### FUNCTION DEFINITIONS #####

// Solve Board
void solveBoard( short int puzzle[BOARDSIZE][BOARDSIZE], int row, int col ){

	// Check if the current value is a zero, then continue
	// Otherwise move to the next value until a non-zero is found
	while( puzzle[row][col] != 0 ){
		if( col < BOARDSIZE - 1 ){
			col = col + 1;
		} else {
			if( row < BOARDSIZE - 1) {
				row = row + 1;
				col = 0;
			}
		}
	}

	short int curVal = 0;
	int sqrRow, sqrCol;
	bool match;

	for( int i = 1; i <= BOARDSIZE; i++ ){
		// Reset the match check bit before iterating through the current value
		match = 0;
		for( int search = 0; search < BOARDSIZE; search++ ){

			// Checks if the current value matches any values in its row or column
			if( puzzle[row][search] == i || puzzle[search][col] == i )
				match = 1;

			// Divide and truncate to find the square the current value fits into
			sqrRow = (int) row / BOARDSQR;
			sqrCol = (int) col / BOARDSQR;

			// Search the current square to see if there is a match
			for( int sR = 0; sR < BOARDSQR; sR++ ){
				for( int sC = 0; sC < BOARDSQR; sC++ ){
					if( puzzle[(sqrRow*BOARDSQR) + sR][sqrCol*BOARDSQR + sC] == i ){
						match = 1;

					}
				}
			}
		}

		// Check if there is a match
		if( match == 0 ){
			// Set the current value since there is no match
			puzzle[row][col] = i;

			// Print the board for testing
			//printBoard( puzzle );

			// Complete recurrsion until the end of the board
			if( col < BOARDSIZE - 1 ){
				solveBoard( puzzle, row, col + 1 );
			} else {
				if( row < BOARDSIZE - 1 ){
					solveBoard( puzzle, row + 1, 0 );
				} else {
					printBoard(puzzle);
				}
			}
			
			// Reset the current value in case the last value was actually wrong
			// Without this, the (0,0) might be wrong but (0,1) is left unchanged
			// when (0,0) iterates to the next value, it would scan (0, 1) and read
			// that value as a match
			puzzle[row][col] = 0;
		}
	}
}

// Print Board
void printBoard( short int puzzle[BOARDSIZE][BOARDSIZE] ){
	int  temp, tempBS, digitsTemp, digitsBS, digitsDiff;
	
	//prints board contents
	for( int i = 0; i < BOARDSIZE; i++ ){
		//If line is the first or any integer multiple of the sqrt of the size of the board
		//create a horizontal line to distinguish the squares
		if( i % BOARDSQR == 0){

			//print out horizontal lines to distinguish squares
			for( int j = 0; j < BOARDSIZE; j++ ){
				//prints _ to cover the | for each number	
				printf("_");

				//dynamically prints _ to cover the number of digits for the largest number the board can have
				//ensures consistent sizing of columns
				temp = BOARDSIZE;
				do{
					temp = (int)(temp/10);
					printf("_");
				} while( temp > 0 );

				//makes a space to create vertical lines between squares
				if( j % BOARDSQR == BOARDSQR - 1)
					printf("_ ");
			}
			//move writehead to next line for printing
			printf("\n");
		}

		for( int j = 0; j < BOARDSIZE; j++ ){
						
			//prints | to visual distinguish integers from one another and then prints the integer
			if( puzzle[i][j] > 0 ) {
				// Checks if there is a value at the current location and prints the value
				printf( "|%d", puzzle[i][j] );
			} else {
				// If there is not a value at the current location, print a space
				printf( "| " );
			}
			
			//print spaces before number to align columns
			temp = (int) puzzle[i][j];
			tempBS = BOARDSIZE;

			digitsTemp = 0;
			digitsBS = 0;

			while((int) temp / 10 > 0){
				temp = (int) temp / 10;
				digitsTemp += 1;
			}

			while( (int)tempBS / 10 > 0 ){
				tempBS = (int) tempBS / 10;
				digitsBS += 1;
			}

			digitsDiff = digitsBS - digitsTemp;

			for( int d = 0; d < digitsDiff; d++ ){
				printf(" ");
			}
	
			//checks for the end of a square and then creates a vertical line to seperate them
			if( j % BOARDSQR == BOARDSQR - 1 ){ printf("| "); }
		}
		printf("\n");
	}
}

