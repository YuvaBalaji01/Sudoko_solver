#include "sudoko.h"
#include "raylib.h" // Assuming Raylib headers are available
#include <cstring>  // For memcpy

// --- Global Puzzle Data ---
int puzzle1[9][9] = {
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

int puzzle2[9][9] = {
    {0, 0, 0, 6, 0, 0, 4, 0, 0},
    {7, 0, 0, 0, 0, 3, 6, 0, 0},
    {0, 0, 0, 0, 9, 1, 0, 8, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 5, 0, 1, 8, 0, 0, 0, 3},
    {0, 0, 0, 3, 0, 6, 0, 4, 5},
    {0, 4, 0, 2, 0, 0, 0, 6, 0},
    {9, 0, 3, 0, 0, 0, 0, 0, 0},
    {0, 2, 0, 0, 0, 0, 1, 0, 0}
};

// Global Pointers and Index
int (*allPuzzles[])[9] = {puzzle1, puzzle2};
const int NUM_PUZZLES = 2;
int currentPuzzleIndex = 0;

// --- Global Variable Definitions (Matching 'extern' in .h) ---
int initialBoard[9][9];
int currentBoard[9][9];
int selectedRow = -1;
int selectedCol = -1;
bool gameSolved = false;
bool isCurrentBoardValid = true; 
bool validationChecked = false;


// --- Solver Function Implementations ---

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

std::pair<int, int> validxy(int x, int y, int matrix[][9]) {
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
            std::pair<int, int> next = validxy(x, y, matrix);
            if (next.first == -1) return true;
            if (validsudoko(next.first, next.second, matrix)) return true;
            matrix[x][y] = 0; // Backtrack
        }
    }
    return false;
}

bool sudoko(int matrix[9][9]) {
    std::pair<int, int> next = validxy(0, 0, matrix);
    if (next.first == -1) return true;
    return validsudoko(next.first, next.second, matrix);
}


// --- Validation and Utility Functions ---

void initializeGame() {
    // Select the current puzzle
    memcpy(initialBoard, allPuzzles[currentPuzzleIndex], sizeof(initialBoard));
    
    // Copy the initial puzzle to the current board for user interaction
    memcpy(currentBoard, initialBoard, sizeof(initialBoard));
    gameSolved = false;
    selectedRow = -1;
    selectedCol = -1;
    isCurrentBoardValid = true; // Reset status
    validationChecked = false; // Reset message
}

void clearUserInputs() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (initialBoard[i][j] == 0) {
                currentBoard[i][j] = 0;
            }
        }
    }
}

bool checkCompleteAndValid() {
    // 1. Check if the board is complete (no zeros)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (currentBoard[i][j] == 0) return false; // Not complete
        }
    }

    // 2. Check validity of every cell
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            int val = currentBoard[r][c];
            currentBoard[r][c] = 0; // Temporarily remove
            bool valid = isvalid(r, c, val, currentBoard);
            currentBoard[r][c] = val; // Restore
            if (!valid) return false;
        }
    }
    return true; // Complete and valid
}


// --- Game Logic and UI Implementations ---

void handleInput() {
    // Reset validation message on any input
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || GetKeyPressed() != 0) {
        validationChecked = false; 
    }
    
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
                // Fixed numbers are BLACK, user inputs are BLUE
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
    const int BUTTON_WIDTH = 120;
    const int BUTTON_HEIGHT = 40;
    const int BUTTON_X = GRID_OFFSET_X + GRID_SIZE * CELL_SIZE + 40;
    const int BUTTON_SPACING = 15;
    int currentY = GRID_OFFSET_Y;

    // --- SOLVE Button ---
    Rectangle solveButton = { (float)BUTTON_X, (float)currentY, (float)BUTTON_WIDTH, (float)BUTTON_HEIGHT };
    Color solveButtonColor = gameSolved ? DARKGRAY : GREEN;
    DrawRectangleRec(solveButton, solveButtonColor);
    DrawText("SOLVE", solveButton.x + 25, solveButton.y + 10, 20, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), solveButton) && !gameSolved) {
        int solveCopy[9][9];
        memcpy(solveCopy, currentBoard, sizeof(currentBoard));

        if (sudoko(solveCopy)) { 
            memcpy(currentBoard, solveCopy, sizeof(currentBoard));
            gameSolved = true;
            validationChecked = false;
        }
    }
    
    // --- CLEAR Button ---
    currentY += BUTTON_HEIGHT + BUTTON_SPACING;
    Rectangle clearButton = { (float)BUTTON_X, (float)currentY, (float)BUTTON_WIDTH, (float)BUTTON_HEIGHT };
    DrawRectangleRec(clearButton, ORANGE);
    DrawText("CLEAR", clearButton.x + 30, clearButton.y + 10, 20, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), clearButton)) {
        clearUserInputs();
        gameSolved = false;
        validationChecked = false;
    }
    
    // --- VALIDATE Button ---
    currentY += BUTTON_HEIGHT + BUTTON_SPACING;
    Rectangle validateButton = { (float)BUTTON_X, (float)currentY, (float)BUTTON_WIDTH, (float)BUTTON_HEIGHT };
    DrawRectangleRec(validateButton, BLUE);
    DrawText("VALIDATE", validateButton.x + 10, validateButton.y + 10, 20, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), validateButton)) {
        isCurrentBoardValid = checkCompleteAndValid();
        validationChecked = true;
        gameSolved = isCurrentBoardValid; // Mark as solved only if it's correct AND complete
    }
    
    // --- NEXT Puzzle Button ---
    currentY += BUTTON_HEIGHT + BUTTON_SPACING;
    Rectangle nextButton = { (float)BUTTON_X, (float)currentY, (float)BUTTON_WIDTH, (float)BUTTON_HEIGHT };
    DrawRectangleRec(nextButton, PURPLE);
    DrawText("NEXT", nextButton.x + 35, nextButton.y + 10, 20, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), nextButton)) {
        currentPuzzleIndex = (currentPuzzleIndex + 1) % NUM_PUZZLES;
        initializeGame(); // Reset game state with the new puzzle
    }

    // --- Status Messages ---
    currentY += BUTTON_HEIGHT + BUTTON_SPACING;

    // 1. Validation Status
    if (validationChecked) {
        Color validationColor = isCurrentBoardValid ? LIME : RED;
        const char* validationText = isCurrentBoardValid ? "CORRECT!" : "INCORRECT/INCOMPLETE!";
        DrawText(validationText, (float)BUTTON_X - 10, (float)currentY, 25, validationColor);
        currentY += 30;
    } 
    // 2. Solve Status (only show if solved by solver and not already shown by validation)
    else if (gameSolved) {
        DrawText("SOLVED!", (float)BUTTON_X + 20, (float)currentY, 25, LIME);
    }
}


// --- Main Function (Game Entry Point) ---

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
