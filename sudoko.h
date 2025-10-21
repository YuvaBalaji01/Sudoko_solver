#ifndef SUDOKU_H
#define SUDOKU_H

#include "raylib.h"
#include <iostream>
#include <utility>

using namespace std;

// --- UI and Game Constants ---
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

// --- Solver Function Prototypes (Declarations) ---
bool isvalid(int x, int y, int d, int matrix[][9]);
pair<int, int> validxy(int x, int y, int matrix[][9]);
bool validsudoko(int x, int y, int matrix[][9]);
bool sudoko(int matrix[9][9]);

// --- Game Logic and UI Prototypes (Declarations) ---
void initializeGame();
void handleInput();
void drawGrid();
void drawButtons();

#endif // SUDOKU_H