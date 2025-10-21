#include "sudoko.h"
#include <cstring> // For memcpy

// --- Global Variable Definitions (Matching declarations in sudoku.h) ---

// The original puzzle (0 means empty)
int initialBoard[9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

int currentBoard[9][9];
int selectedRow = -1;
int selectedCol = -1;
bool gameSolved = false;

// --- Solver Function Implementations (Your original code) ---

bool isvalid(int x, int y, int d, int matrix[][9]) {
    for (int i = 0; i < 9; i++) {
        if (matrix[i][y] == d || matrix[x][i] == d) return false;
    }

    int row = x - x % 3;
    int col = y - y % 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix[row + i][col + j] == d) return false;
        }
    }
    return true;
}

pair<int, int> validxy(int x, int y, int matrix[][9]) {
    for (int i = x; i < 9; i++) {
        for (int j = (i == x ? y : 0); j < 9; j++) {
            if (matrix[i][j] == 0) return {i, j};
        }
    }
    return {-1, -1};
}

bool validsudoko(int x, int y, int matrix[][9]) {
    if (x == -1) {
        return true;
    }

    for (int i = 1; i <= 9; i++) {
        if (isvalid(x, y, i, matrix)) {
            matrix[x][y] = i;
            pair<int, int> next = validxy(x, y, matrix);
            if (next.first == -1) return true;
            if (validsudoko(next.first, next.second, matrix)) return true;
            matrix[x][y] = 0; // Backtrack
        }
    }
    return false;
}

bool sudoko(int matrix[9][9]) {
    pair<int, int> next = validxy(0, 0, matrix);
    if (next.first == -1) return true;
    return validsudoko(next.first, next.second, matrix);
}


// --- Game Logic and UI Implementations ---

void initializeGame() {
    // Copy the initial puzzle to the current board for user interaction
    memcpy(currentBoard, initialBoard, sizeof(initialBoard));
    gameSolved = false;
    selectedRow = -1;
    selectedCol = -1;
}

void handleInput() {
    // Cell Selection Logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePoint = GetMousePosition();
        if (mousePoint.x >= GRID_OFFSET_X && mousePoint.x < GRID_OFFSET_X + GRID_SIZE * CELL_SIZE &&
            mousePoint.y >= GRID_OFFSET_Y && mousePoint.y < GRID_OFFSET_Y + GRID_SIZE * CELL_SIZE)
        {
            selectedCol = (mousePoint.x - GRID_OFFSET_X) / CELL_SIZE;
            selectedRow = (mousePoint.y - GRID_OFFSET_Y) / CELL_SIZE;

            // Only allow selection of initially empty cells
            if (initialBoard[selectedRow][selectedCol] != 0) {
                selectedRow = -1;
                selectedCol = -1;
            }
        } else {
            selectedRow = -1; // Deselect if clicked outside
            selectedCol = -1;
        }
    }

    // Number Input Logic
    if (selectedRow != -1 && selectedCol != -1) {
        int key = GetKeyPressed();
        if (key >= KEY_ONE && key <= KEY_NINE) {
            currentBoard[selectedRow][selectedCol] = key - KEY_ZERO;
        }
        else if (key == KEY_BACKSPACE || key == KEY_DELETE || key == KEY_ZERO) {
            currentBoard[selectedRow][selectedCol] = 0; // Clear the cell
        }
    }
}

void drawGrid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            Rectangle cellRect = {
                (float)(GRID_OFFSET_X + j * CELL_SIZE), (float)(GRID_OFFSET_Y + i * CELL_SIZE),
                (float)CELL_SIZE, (float)CELL_SIZE
            };

            // 1. Highlight Selected Cell
            if (i == selectedRow && j == selectedCol) {
                DrawRectangleRec(cellRect, SKYBLUE);
            }

            // 2. Draw Number
            if (currentBoard[i][j] != 0) {
                char numText[2] = { (char)('0' + currentBoard[i][j]), '\0' };
                Color textColor = (initialBoard[i][j] != 0) ? BLACK : BLUE; 
                
                int textWidth = MeasureText(numText, FONT_SIZE);
                int textX = cellRect.x + (CELL_SIZE - textWidth) / 2;
                int textY = cellRect.y + (CELL_SIZE - FONT_SIZE) / 2;

                DrawText(numText, textX, textY, FONT_SIZE, textColor);
            }

            // 3. Draw Cell Border
            DrawRectangleLinesEx(cellRect, 1, DARKGRAY);
        }
    }

    // 4. Draw Thick 3x3 Block Lines
    for (int k = 0; k <= GRID_SIZE; k += 3) {
        float x_offset = GRID_OFFSET_X + k * CELL_SIZE;
        float y_offset = GRID_OFFSET_Y + k * CELL_SIZE;
        float grid_end_x = GRID_OFFSET_X + GRID_SIZE * CELL_SIZE;
        float grid_end_y = GRID_OFFSET_Y + GRID_SIZE * CELL_SIZE;

        DrawLineEx({x_offset, (float)GRID_OFFSET_Y}, {x_offset, grid_end_y}, 3, BLACK); // Vertical
        DrawLineEx({(float)GRID_OFFSET_X, y_offset}, {grid_end_x, y_offset}, 3, BLACK); // Horizontal
    }
}

void drawButtons() {
    Rectangle solveButton = { GRID_OFFSET_X + GRID_SIZE * CELL_SIZE + 40, GRID_OFFSET_Y, 120, 40 };
    Color buttonColor = gameSolved ? GRAY : GREEN;

    DrawRectangleRec(solveButton, buttonColor);
    DrawText("SOLVE", solveButton.x + 25, solveButton.y + 10, 20, WHITE);

    // Check for click on solve button
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), solveButton) && !gameSolved) {
        // Create a copy to solve, just in case the original needs to be preserved
        int solveCopy[9][9];
        memcpy(solveCopy, currentBoard, sizeof(currentBoard));

        if (sudoko(solveCopy)) { 
            // If solved, copy the solution back to the current board
            memcpy(currentBoard, solveCopy, sizeof(currentBoard));
            gameSolved = true;
        } else {
            // No solution exists for the current confidguration
            // You can add a temporary message display here if needed
        }
    }

    if (gameSolved) {
        DrawText("SOLVED!", solveButton.x + 20, solveButton.y + 60, 25, LIME);
    }
}


int main() {
    // Initialize Raylib Window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sudoku Game Solver");
    SetTargetFPS(60);

    initializeGame(); // Set up the puzzle

    // Main Game Loop
    while (!WindowShouldClose()) {

        // --- UPDATE (Logic and Input) ---
        handleInput();

        // --- DRAW (Rendering) ---
        BeginDrawing();
        ClearBackground(RAYWHITE); 

        drawGrid();
        drawButtons();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}