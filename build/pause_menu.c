
#include "pause_menu.h"
#include "raylib.h"

// Pause menu state
const char *pauseMenuOptions[] = { "Resume", "Main Menu", "Controls", "Quit" };
static int selectedPauseOption = 0;

void DrawPauseMenu(GameState *gameState) {
    // Semi-transparent black overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){ 0, 0, 0, 150 });

    // Menu Options
    for (int i = 0; i < 4; i++) {
        Color textColor = (i == selectedPauseOption) ? RED : WHITE;
        DrawText(pauseMenuOptions[i], SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50 + i * 40, 30, textColor);
    }

    // Input and navigation
    if (IsKeyPressed(KEY_DOWN)) {
        selectedPauseOption = (selectedPauseOption + 1) % 4; // Move to the next option
    }
    if (IsKeyPressed(KEY_UP)) {
        selectedPauseOption = (selectedPauseOption - 1 + 4) % 4; // Move to the previous option
    }

    // Selections
    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedPauseOption) {
            case 0: // Resume
                *gameState = GAMEPLAY;
                break;
            case 1: // Main Menu
                *gameState = MAIN_MENU;
                break;
            case 2: // Controls
                *gameState = CONTROLS;
                break;
            case 3: // Quit
                CloseWindow(); // Close the game window
                exit(0);       // Exit the application
        }
    }
}