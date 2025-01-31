
#include "menu.h"
#include "config.h"

GameState displayMenu() {
    int selectedOption = 0;
    const char *menuOptions[] = { "Easy", "Medium", "Hard", "Controls", "Quit" };
    const int optionsCount = 5;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("ARKANOIDER", SCREEN_WIDTH / 2 - 150, 100, 40, CYAN);

        // Menu options
        for (int i = 0; i < optionsCount; i++) {
            Color textColor = (i == selectedOption) ? YELLOW : WHITE;
            DrawText(menuOptions[i], SCREEN_WIDTH / 2 - 100, 250 + i * 50, 30, textColor);
        }

        // Menu navigation
        if (IsKeyPressed(KEY_DOWN)) {
            selectedOption = (selectedOption + 1) % optionsCount;
        }
        if (IsKeyPressed(KEY_UP)) {
            selectedOption = (selectedOption - 1 + optionsCount) % optionsCount;
        }

        // Menu selection
        if (IsKeyPressed(KEY_ENTER)) {
            switch (selectedOption) {
                case 0: return GAMEPLAY;  // Easy
                case 1: return GAMEPLAY;  // Medium
                case 2: return GAMEPLAY;  // Hard
                case 3: return CONTROLS;
                case 4: return QUIT;
            }
        }

        EndDrawing();
    }

    return QUIT; // Exit if window is closed
}

void DisplayControls() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawText("CONTROLS", SCREEN_WIDTH / 2 - 100, 100, 40, WHITE);
        DrawText("Left Arrow: Move paddle Left", SCREEN_WIDTH / 2 - 200, 200, 20, WHITE);
        DrawText("Right Arrow: Move paddle Right", SCREEN_WIDTH / 2 -200, 250, 20, WHITE);
        DrawText("Press BACKSPACE to go back", SCREEN_WIDTH / 2 - 200, 300, 20, WHITE);
        
        if (IsKeyPressed(KEY_BACKSPACE)) {
            return;
        }
        
        EndDrawing();
    }
}



























