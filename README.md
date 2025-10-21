Sudoku Solver Game (C++ and Raylib)
This is a graphical Sudoku game built in C++ using the Raylib library. It features a playable grid and a backtracking solver function.

How to Build and Run (Windows/MinGW)
Since this project uses the Raylib game library, it requires the correct C++ compiler environment to link successfully. The recommended method for Windows is to use the MSYS2 package manager, which guarantees compatibility.

Prerequisites
You must have the following software installed:

Git (for cloning the repository).

Visual Studio Code (or any C++ IDE/editor).

MSYS2: A software distribution platform for building native Windows applications.

Setup Instructions
Follow these steps to set up your environment, install dependencies, and build the game:

Step 1: Install MSYS2 and Dependencies
Open the MSYS2 terminal (search for "MSYS2 UCRT64" in the Start Menu). All subsequent commands must be run within this terminal.

Update Core Packages:

Bash
command:
pacman -Syu

Install Compiler (GCC) and Raylib: This command installs the modern MinGW-w64 compiler and the compatible Raylib library package.

Bash
command :
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-raylib

Step 2: Configure VS Code and Build
Clone the Repository: Navigate to where you want to store the project (e.g., your Desktop) and clone the repository:

Bash

git clone [YOUR_REPO_URL]
cd [YOUR_REPO_NAME]
Configure System Path (One-Time Setup): For VS Code to find the new compiler, you need to add the MSYS2 binary folder to your Windows System PATH environment variables:

Path to add: C:\msys64\ucrt64\bin

(You must restart VS Code after adding the path.)

Build the Game: The project includes a pre-configured .vscode/tasks.json file with the correct build command.

Open the project folder in VS Code.

Press Ctrl + Shift + B (the default shortcut for "Run Build Task").

Step 3: Run the Game
After a successful build, the executable file (sudoku_game.exe) will be created in the project root directory.

Run the game directly from the VS Code terminal:

Bash

.\sudoku_game.exe
