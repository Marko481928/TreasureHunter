#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// ANSI escape codes for text colors
#define RESET   "\033[0m"

// Basic Colors
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m" 
#define CYAN    "\033[0;36m" 
#define WHITE   "\033[0;37m"

// Bright Colors
#define B_BLACK   "\033[1;30m" 
#define B_RED     "\033[1;31m"
#define B_GREEN   "\033[1;32m"
#define B_YELLOW  "\033[1;33m"
#define B_BLUE    "\033[1;34m"
#define B_MAGENTA "\033[1;35m"
#define B_CYAN    "\033[1;36m"
#define B_WHITE   "\033[1;37m"

// GLOBAL CONSTANTS & FUNCTION PROTOTYPES
// Using a 7x7 array so we can use 1-based indexing (1 to 6) for easier user input
const int ROW = 7, COL = 7;

void clearScreen();
void printMaze(char displayGrid[ROW][COL]);
void randomize(int* pRow, int* pCol, int grid[ROW][COL]);
int mvmnt(int* pRow, int* pCol, int grid[ROW][COL], char displayGrid[ROW][COL]);

int main() {
    char password[6] = "12345"; 
    char input[20]; // Buffer for user input (larger than needed to prevent overflow)
    
    clearScreen();
    
    // Print ASCII Art Game Title
    printf(YELLOW"\n");
    printf("         ▲▲▲▲▲▲▼▼▼▼▼▼▼▼▲▲▲▲▲▲▲┌┬┐\n");
    printf("       ┌────────┬───────┬─────┼┼┼\n");
    printf("       ├                      └┼┘\n");
    printf("       │ Bitmaze Treasure Hunt │ \n");
    printf("      ┌┼┐                      │ \n");
    printf("      ┼┼┼─┴─────────┴─────┴────┘ \n");
    printf("      └┼┘V▼▼▼▼▲▲▲▲▲▲▲▲▼▼▼▼▼▼V \n\n"RESET);

    // Infinite loop until the correct password is provided
    while (1) {
        printf(BLUE"      [!] Enter the 5-character password: "RESET);
        scanf("%s", input); 
        
        // strcmp returns 0 if both strings are exactly identical
        if (strcmp(password, input) == 0) {
            printf(GREEN"      [+] Access granted!\n\n"RESET);
            break; // Exit the authentication loop
        } else {
            printf(RED"      [-] Wrong password. Try again.\n"RESET);
        }
    }

    // Player Setup
    char p1Name[50], p2Name[50];
    int p1Wins = 0, p2Wins = 0; // Track overall session wins
    
    printf(B_MAGENTA"      [>] Enter Player 1 name: ");
    scanf("%s", p1Name);
    printf("      [>] Enter Player 2 name: ");
    scanf("%s", p2Name);
	printf(RESET);

    char playAgain;
    
    // MAIN SESSION LOOP (Executes once per match)
    do {
        int field[ROW][COL];        // Hidden grid containing bitwise treasure data
        char displayGrid[ROW][COL]; // Visible grid containing 'X', '.', 'G', 'T'
        int p1Score = 0, p2Score = 0;
        int turn = 0;               // Determines whose turn it is
        int totalMoves = 0;         // Tracks how many cells have been opened
        int r = 1, c = 1;           // Row and Column variables
        
        // Seed the random number generator using the current system time
        srand((unsigned int)time(0));
        
        // Set the hidden field to random values
        randomize(&r, &c, field);

        // Initialize the visible display grid with '.' (unexplored cells)
        for (int i = 1; i < 7; i++) {
            for (int j = 1; j < 7; j++) {
                displayGrid[i][j] = '.'; 
            }
        }

        // GAMEPLAY TURN LOOP
        // Continues until a player reaches 50 points OR all 36 cells are opened
        while (p1Score < 50 && p2Score < 50 && totalMoves < 36) {
            clearScreen();
            
            // Determine the current player and their score pointer based on odd/even turn
            char* currentPlayer = (turn % 2 == 0) ? p1Name : p2Name;
            int* currentScore = (turn % 2 == 0) ? &p1Score : &p2Score;

            // Render Scoreboard
            printf(B_CYAN"\n");
            printf("        ▲┌──►  ┬┐┌────┬──────────────┬────────┬────────┐  \n");
            printf("        ││SCORE││├─► %s (%d) | %s (%d)\n", p1Name, p1Score, p2Name, p2Score);
            printf("        └┴  ◄──┘▼└───────┴─────────────────┴──────┴──────┘\n"RESET);
            
            // Render the ASCII Maze
            printMaze(displayGrid);

            // Prompt for coordinates
            printf("      ─<◄◄ %s's Turn ►►>─\n", currentPlayer);
            printf("      [?] Enter row and col (1-6): ");
            scanf("%d %d", &r, &c);

            // Input Validation 1: Ensure coordinates are within the 1-6 range
            if (r < 1 || r > 6 || c < 1 || c > 6) {
                printf("      [!] Out of range! Press Enter to try again...");
                getchar(); getchar(); // Wait for user to press Enter
                continue; // Restart the loop
            }

            // Input Validation 2: Ensure the cell hasn't been selected previously
            if (displayGrid[r][c] != '.') {
                printf("      [!] Cell already selected! Press Enter...");
                getchar(); getchar(); 
                continue; 
            }

            // Execute the move: Check cell data, update grid, and calculate score change
            int scoreChange = mvmnt(&r, &c, field, displayGrid);
            
            // Apply the returned score change to the current player's score
            *currentScore += scoreChange;
            totalMoves++; // Increment the counter for opened cells

            // Pause the screen so the user can read what they found
            printf("      Press Enter to continue...");
            getchar(); getchar(); 

            // Pass the turn to the next player
            turn++;
        }

        // MATCH OVER - RESULTS EVALUATION
        clearScreen();
        printf(RED"\n        │              │\n");
        printf("        ││             ││\n");
        printf("        ▼│▼ GAME OVER!▼│▼\n");
        printf("         ▼             ▼ \n"RESET);
        printf(YELLOW"\n      Final Score: %s (%d) | %s (%d)\n\n"RESET, p1Name, p1Score, p2Name, p2Score);

        // Determine the winner (handling standard wins and ties if the board is full)
        if (p1Score >= 50 || p1Score > p2Score) {
            printf(GREEN"      ─<◄◄ %s WINS! ►►>─\n"RESET, p1Name);
            p1Wins++;
        } else if (p2Score >= 50 || p2Score > p1Score) {
            printf(GREEN"      ─<◄◄  %s WINS! ►►>─\n"RESET, p2Name);
            p2Wins++;
        } else {
            printf(GREEN"      ─<◄◄  IT'S A TIE! ►►>─\n"RESET);
        }

        // Display overall session statistics
        printf("\n      ───Total─Wins───► %s: %d | %s: %d\n", p1Name, p1Wins, p2Name, p2Wins);
        
        // Ask players if they want to restart the match loop
        printf(B_BLUE"\n      [?] Do you want to play again? (y/n): "RESET);
        scanf(" %c", &playAgain);

    } while (playAgain == 'y' || playAgain == 'Y');

    printf("\n      Thanks for playing " B_CYAN "BitMaze!\n\n"RESET);
    return 0;
}

// Clears the terminal screen using ANSI escape sequences
// \e[1;1H moves cursor to top left, \e[2J clears the entire screen
void clearScreen() {
    printf("\e[1;1H\e[2J");
}

// MAZE RENDERER
void printMaze(char displayGrid[ROW][COL]) {
    printf(B_WHITE"\n");
    
    printf("            1   2   3   4   5   6 \n");
    printf("          ┌---+---+---+---+---+---┐\n");
    
    for (int i = 1; i < 7; i++) {
        printf("        %d ├", i); 
        
        for (int j = 1; j < 7; j++) {
            // Apply specific colors based on the character inside the cell
            if (displayGrid[i][j] == 'T') {
                printf(B_RED);
            } else if (displayGrid[i][j] == 'G') {
                printf(B_YELLOW);
            } else if (displayGrid[i][j] == 'X') {
                printf(BLACK);
            }
            
            // Print the actual character
            printf("►%c◄" RESET, displayGrid[i][j]);
            
            // Print the vertical separator, except for the last column
            if (j < 6) {
                printf(B_WHITE"┼"RESET);
            }
        }
        
        // Print the right border
        printf(B_WHITE"┤\n");
        
        // Print horizontal row separators, or the bottom border for the last row
        if (i < 6) {
            printf("          +───┼───┼───┼───┼───┼───+\n");
        } else {
            printf("          └---+---+---+---+---+---┘\n");
        }
    }
    printf("\n"RESET);
}

// Populates the hidden grid with random 8-bit integers (1 to 255)
// Uses pointers to modify the row/col variables, as initially designed by partner
void randomize(int* pRow, int* pCol, int grid[ROW][COL]) {
    int Rows = *pRow;
    int Columns = *pCol;
    for (Rows = 1; Rows < 7; Rows++) {
        for (Columns = 1; Columns < 7; Columns++) {
            grid[Rows][Columns] = (rand() % 255) + 1;
            *pRow = Rows;
            *pCol = Columns;
        }
    }
}

// Processes the selected cell, decodes the bitwise data, and updates the grid
int mvmnt(int* pRow, int* pCol, int grid[ROW][COL], char displayGrid[ROW][COL]) {
    int r = *pRow;
    int c = *pCol;
    int cellValue = grid[r][c];
    int scoreChange = 0;

    // Decoding the secret data using bitwise AND operations
    int isTrap = cellValue & 1; // Checks if Bit 0 is set (0000 0001)
    int isGold = cellValue & 2; // Checks if Bit 1 is set (0000 0010)

    // Evaluate cell contents based on decoded bits
    if (isTrap) {
        displayGrid[r][c] = 'T'; // Mark cell as a Trap
        scoreChange = -1;        // Deduct 1 point     
        printf("\n      >>> Oh no! It's a " B_RED "TRAP!" RESET " (-1 point)\n");
    } 
    else if (isGold) {
        displayGrid[r][c] = 'G'; // Mark cell as Gold
        // Bitwise right shift by 4 moves bits 4-7 to the position of bits 0-3
        // This isolates the gold amount value (0-15)
        int goldAmount = cellValue >> 4; 
        scoreChange = goldAmount; // Add gold amount to the score
        printf("\n      >>> Awesome! You found " B_YELLOW "GOLD!" RESET " (+%d points)\n", goldAmount);
    } 
    else {
        displayGrid[r][c] = 'X'; // Mark cell as visited but empty
        printf("\n      >>> Nothing here. Just an " B_BLACK "empty cell." RESET "\n");
    }

    return scoreChange; // Return the calculated value to update the main score
}