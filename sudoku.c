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
	int row;
	int col;

	//link to next node
	struct node* next;	
	//link to previous node
	struct node* prev;
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

    // Prompt user for their board inputs
    bool valAddDone = 0;

    // Initialize row col and value temp values
    int r = 0, c = 0, v = 1;

    // Initialize the completed char
    char done;
    while( !valAddDone ){
        // Print current board state
        printBoard( puzzle );

        // Prompt user for row, col, and value of each known number of the puzzle
        // TODO: Validity check these inputs
        printf("Please input the column of the number.\n");
        scanf("%d", &c);
        printf("Please input the row of the number.\n");
        scanf("%d", &r);
        printf("Please input the number.\n");
        scanf("%d", &v);
        printf("If you would like to add another number type 'n' or 'N'. Otherwise type 'y' or 'Y' for the puzzle to be solved.\n");
        scanf(" %c", &done );

        // Do the loop again for the user to input another value if they want
        puzzle[BOARDSIZE - r][c - 1] = v;
        if( done == 'n' || done == 'N' ){
            valAddDone = 0;
        } else {
            if( done == 'y' || done == 'Y' ){
                valAddDone = 1;
            }
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

	short int curVal = 0;
	int sqrRow, sqrCol, count = 0;
	bool match;

	// Count the number of unknown values in the puzzle
	for( int i = 0; i < BOARDSIZE; i++ ){
		for( int j = 0; j < BOARDSIZE; j++ ){
			if( puzzle[i][j] == 0 )
				count++;
		}
	}

	// Create the head of the linked list
	// The linked list will contain the rows and columns for each of the unknown values
	node_val * head = NULL;
	head = (node_val *) calloc(1, sizeof(node_val));

	// Initialize the first value of the linked list and set it to the beggining of the puzzle
	head->val = 0;
	head->row = 0;
	head->col = 0;
    head->next = NULL;
    head->prev = NULL;

    // Initialize the value c (this will account for how many unkown value nodes we need to create)
    int c = 0;

    // Set the current node pointer to the head of the linked list
    node_val *curNode = head;

    // Initialize the reset boolean (this will account for if we have exhausted every possible value for an unkown)
    bool reset = 0;

    // Initialize the iterater short int (this will be used to check if a value is viable before writing it into the puzzle and the linked list)
    int i = 1;
	// Iterate through and generate a node in the linked list for every unknown value
	while( c < count ){

        // Only find the next unknown value if the linked list does not already contain it 
        if( reset == 0 ){
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
                } // End of else statement
            } // End of while loop

            // Assign the unkown values row and column data to the newest linked list node
            curNode->row = row;
            curNode->col = col;
        }

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
            curNode->val = i;
            puzzle[row][col] = curNode->val;

            // Set i to 1 so that the next unknown can start being iterated through to check for matches
            i = 1;

            // If at the end of the linked list and all unknowns are not already contained within the linked list
            if( c != count && curNode->next == NULL){
                // Create a new linked list node for the next unkown
                node_val *nextNode = (node_val *) calloc(1, sizeof(node_val) ); 
                nextNode->prev = curNode;
                nextNode->next = NULL;

                // Add the new node to the end of the linked list and move to it
                curNode->next = nextNode;
                curNode = curNode->next;

                // Increase the count of unknowns in the linked list by 1
                c++;
            } else {

                // If all unknowns have not been accounted for, but the next unknown is in the linked list
                if( c != count )
                    // Move to the next unknown in the linked list
                    curNode = curNode->next;
                    
                    // Since this state can only be achieved by the next node matching for all possible
                    // values, the reset bit had to have been set. Since the next match for the current
                    // node has been found, the program can progress to the next node again.
                    reset = 0;
            }
        

        } else {
            // Reset the current value in case the last value was actually wrong
            // Without this, the (0,0) might be wrong but (0,1) is left unchanged
            // when (0,0) iterates to the next value, it would scan (0, 1) and read
            // that value as a match
            
            // Since the previous unknown could be set to the max value, the program needs to loop back to
            // the last unknown that is not set to the max value. Additionally, as the program loops back
            // each value needs to be reset to 0 in order to be looped through again 
            while( i >= BOARDSIZE ) {
                curNode->val = 0;
                puzzle[row][col] = 0;
                curNode = curNode->prev;

                // Since the program iterated back to a previous linked list value
                // the temp value (i, row, and col) need to be updated to match
                i = curNode->val;
                row = curNode->row;
                col = curNode->col;

                // Engage the reset bit so that the program does not loop through to the next unknown value
                // wasting CPU clock cycles
                reset = 1;
            }
            // Increase the match check value by 1
            i = i+1;
        }
	} // End of while loop
    //Print the final completed board
    printBoard(puzzle);

    // Loop to the end of the linked list
    curNode = head;
    while( curNode->next != NULL ){
        // Move forward 1 node
        curNode = curNode->next;
        // Free the previous node
        free( curNode->prev );
    }
    // Free the final node in the linked list
    free( curNode );
}

// Print Board
void printBoard( short int puzzle[BOARDSIZE][BOARDSIZE] ){
    int temp, tempBS, digitsTemp, digitsDiff, digitsBS = 0;

    tempBS = BOARDSIZE;

    while((int) tempBS / 10 > 0){
        tempBS = (int) tempBS / 10;
        digitsBS += 1;
    }

    //prints board contents
    for( int i = 0; i < BOARDSIZE; i++ ){
        //If line is th efirst or any integer multiple of the sqrt of the size of the board
        //create a horizontal line to distinguish the squares
        if( i % BOARDSQR == 0){

            // TODO: Adjust this to print dynamic amount of spaces to acoomodate for the far left column
            // listing out the number of rows on the board
            temp = 0;
            do{
                printf(" ");
                temp++;
            } while( temp < digitsBS );
            printf(" ");

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

        // Print spaces for dynamically to account for multi-digit boards
        temp = BOARDSIZE -  i;
        digitsTemp = 0;

        while( (int)( temp / 10 )> 0 ){
          temp = (int) temp / 10;  
          digitsTemp += 1;
        } 

        digitsDiff = digitsBS - digitsTemp;

        temp = 0;
        while( temp < digitsDiff){
            printf(" ");
            temp++;
        }

        printf("%d ", BOARDSIZE - i);

        for( int j = 0; j < BOARDSIZE; j++ ){

            //prints | to visually distinguish integers from one another and then prints the integer
            if( puzzle[i][j] > 0 ){
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

            while((int) temp / 10 > 0){
                temp = (int) temp / 10;
                digitsTemp += 1;
            }
            digitsDiff = digitsBS - digitsTemp;

            for( int d = 0; d < digitsDiff; d++ ){
                printf( " " );
            }

            //Checks for the end of a square and then creates a vertical line to seperate them
            if( j % BOARDSQR == BOARDSQR - 1)
                printf("| ");
        }
        printf("\n");
    }

    // TODO: Fix to print dynamic amount of spaces to acoomodate for the far left column
    // listing out the number of rows on the board
    temp = 0;
    do{
        printf(" ");
        temp++;
    } while( temp < digitsBS );
    printf(" ");

    //Print the row of values to represent the columns for the user to see
    for( int i = 1; i <= BOARDSIZE; i++ ){

        temp = i;

        while( (int)( temp / 10 )> 0 ){
            temp = (int) temp / 10;  
            digitsTemp += 1;
        } 

        digitsDiff = digitsBS - digitsTemp;

        temp = 0;

        while( temp < digitsDiff ){
            printf(" ");
            temp++;
        }

        printf(" %d", i);

        //makes a space to create vertical lines between squares
        if( ( i - 1 ) % BOARDSQR == BOARDSQR - 1)
            printf("  ");
    }

    printf("\n");
}
