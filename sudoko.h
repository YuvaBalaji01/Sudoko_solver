#ifndef SUDOKU_H
#define SUDOKU_H

#include <utility> // For std::pair

// --- UI and Game Constants ---
// Raylib is assumed to be included in the implementation file
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 9;
const int CELL_SIZE = 50; 
const int FONT_SIZE = 30;
const int GRID_OFFSET_X = (SCREEN_WIDTH - GRID_SIZE * CELL_SIZE) / 2;
const int GRID_OFFSET_Y = (SCREEN_HEIGHT - GRID_SIZE * CELL_SIZE) / 2;

// --- External Game State Declarations ---
extern int initialBoard[9][9];
extern int currentBoard[9][9];
extern int selectedRow;
extern int selectedCol;
extern bool gameSolved;
extern bool isCurrentBoardValid; // New: Stores the result of the validation check
extern bool validationChecked;   // New: True if the Validate button was just pressed
extern int currentPuzzleIndex;   // New: Tracks the current puzzle loaded

// --- Solver Function Prototypes ---
bool isvalid(int x, int y, int d, int matrix[][9]);
std::pair<int, int> validxy(int x, int y, int matrix[][9]);
bool validsudoko(int x, int y, int matrix[][9]);
bool sudoko(int matrix[9][9]);
bool checkCompleteAndValid(); // New: Checks if the current board is a correct solution

// --- Game Logic and UI Prototypes ---
void initializeGame();
void handleInput();
void drawGrid();
void drawButtons();
void clearUserInputs(); // New: Clears only the user-entered numbers

#endif // SUDOKU_H
