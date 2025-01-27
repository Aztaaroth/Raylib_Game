#include "raylib.h"
#include "menu.h"       // Main menu functions
#include "pause_menu.h" // Pause menu functions
#include "game_state.h" // Include the shared GameState enum
#include "config.h"
#include <stdlib.h>
#include <time.h>

typedef struct Ball {
    Vector2 position;
    Vector2 velocity;
    float radius;
} Ball;

typedef struct Paddle {
    Vector2 position;
    float width;
    float height;
} Paddle;

typedef struct Brick {
    Rectangle rect;
    bool active;
} Brick;

typedef struct PowerUp {
    Vector2 position;
    bool active;
    int type; // 1 for extra life
} PowerUp;

// Function prototypes
void InitializeBricks(Brick bricks[BRICK_ROWS][BRICK_COLUMNS]);
void SpawnPowerUp(PowerUp powerUps[], Vector2 position);
void ResetBall(Ball *ball, Paddle paddle);

int main() {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkanoider");
    SetTargetFPS(60);
    srand(time(NULL));
    
    // Game State
    GameState gameState = MENU;

    while (!WindowShouldClose()) {
        switch (gameState) {
            case MENU:
                gameState = displayMenu();
                break;

            case CONTROLS:
                DisplayControls();
                gameState = MENU;
                break;

            case GAMEPLAY: {
                // Game objects
                Ball ball = {
                    .position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
                    .velocity = { 300.0f, -300.0f },
                    .radius = BALL_RADIUS
                };

                Paddle paddle = {
                    .position = { (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f, SCREEN_HEIGHT - 50 },
                    .width = PADDLE_WIDTH,
                    .height = PADDLE_HEIGHT
                };

                Brick bricks[BRICK_ROWS][BRICK_COLUMNS];
                InitializeBricks(bricks);

                PowerUp powerUps[MAX_POWERUPS] = { 0 };
                int lives = 3;
                int bricksRemaining = BRICK_ROWS * BRICK_COLUMNS;

                // Main game loop
                while (gameState == GAMEPLAY && !WindowShouldClose()) {
                    // Handle pause menu
                    if (IsKeyPressed(KEY_ESCAPE)) {
                        gameState = PAUSE_MENU;
                    }

                    // Update
                    // Ball movement
                    ball.position.x += ball.velocity.x * GetFrameTime();
                    ball.position.y += ball.velocity.y * GetFrameTime();

                    // Ball collision with walls
                    if (ball.position.x <= 0 || ball.position.x >= SCREEN_WIDTH) {
                        ball.velocity.x *= -1;
                    }
                    if (ball.position.y <= 0) {
                        ball.velocity.y *= -1;
                    }

                    // Ball collision with paddle
                    Rectangle paddleRect = { paddle.position.x, paddle.position.y, paddle.width, paddle.height };
                    if (CheckCollisionCircleRec(ball.position, ball.radius, paddleRect)) {
                        ball.velocity.y *= -1;
                        ball.position.y = paddle.position.y - ball.radius; // Prevent sticking
                    }

                    // Ball out of bounds (lose a life)
                    if (ball.position.y >= SCREEN_HEIGHT) {
                        lives--;
                        if (lives <= 0) {
                            // Game over
                            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 40, RED);
                            EndDrawing();
                            CloseWindow();
                            return 0;
                        } else {
                            ResetBall(&ball, paddle);
                        }
                    }

                    // Ball collision with bricks
                    for (int i = 0; i < BRICK_ROWS; i++) {
                        for (int j = 0; j < BRICK_COLUMNS; j++) {
                            if (bricks[i][j].active && CheckCollisionCircleRec(ball.position, ball.radius, bricks[i][j].rect)) {
                                bricks[i][j].active = false;
                                ball.velocity.y *= -1;
                                bricksRemaining--;

                                // 10% chance to spawn a power-up
                                if (rand() % 10 == 0) {
                                    SpawnPowerUp(powerUps, (Vector2){ bricks[i][j].rect.x + BRICK_WIDTH / 2, bricks[i][j].rect.y + BRICK_HEIGHT / 2 });
                                }

                                break;
                            }
                        }
                    }

                    // Paddle movement
                    if (IsKeyDown(KEY_LEFT) && paddle.position.x > 0) {
                        paddle.position.x -= 600.0f * GetFrameTime();
                    }
                    if (IsKeyDown(KEY_RIGHT) && paddle.position.x < SCREEN_WIDTH - paddle.width) {
                        paddle.position.x += 600.0f * GetFrameTime();
                    }

                    // Power-up movement and collection
                    for (int i = 0; i < MAX_POWERUPS; i++) {
                        if (powerUps[i].active) {
                            powerUps[i].position.y += 600.0f * GetFrameTime(); // Move down

                            // Check if collected by the paddle
                            if (CheckCollisionCircleRec(powerUps[i].position, BALL_RADIUS, paddleRect)) {
                                if (powerUps[i].type == 1) {
                                    lives++;
                                }
                                powerUps[i].active = false; // Deactivate power-up
                            }

                            // Deactivate if it falls off the screen
                            if (powerUps[i].position.y > SCREEN_HEIGHT) {
                                powerUps[i].active = false;
                            }
                        }
                    }

                    // Check win condition
                    if (bricksRemaining == 0) {
                        DrawText("YOU WIN!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 40, GREEN);
                        EndDrawing();
                        CloseWindow();
                        return 0;
                    }

                    // Draw
                    BeginDrawing();
                    ClearBackground(BLACK);

                    // Draw ball
                    DrawCircleV(ball.position, ball.radius, WHITE);

                    // Draw paddle
                    DrawRectangleV(paddle.position, (Vector2){ paddle.width, paddle.height }, WHITE);

                    // Draw bricks
                    for (int i = 0; i < BRICK_ROWS; i++) {
                        for (int j = 0; j < BRICK_COLUMNS; j++) {
                            if (bricks[i][j].active) {
                                DrawRectangleRec(bricks[i][j].rect, BLUE);
                            }
                        }
                    }

                    // Draw power-ups
                    for (int i = 0; i < MAX_POWERUPS; i++) {
                        if (powerUps[i].active) {
                            DrawCircleV(powerUps[i].position, BALL_RADIUS, GREEN); // Power-ups are green
                        }
                    }

                    // Draw lives
                    DrawText(TextFormat("Lives: %d", lives), 10, 10, 20, WHITE);

                    EndDrawing();
                }
                break;
            }

            case PAUSE_MENU:
                DrawPauseMenu(&gameState);
                break;

            case QUIT:
                CloseWindow();
                return 0;
        }
    }
    
    CloseWindow();
    return 0;
}

void InitializeBricks(Brick bricks[BRICK_ROWS][BRICK_COLUMNS]) {
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLUMNS; j++) {
            bricks[i][j].rect.x = j * (BRICK_WIDTH + 10) + 35;
            bricks[i][j].rect.y = i * (BRICK_HEIGHT + 10) + 50;
            bricks[i][j].rect.width = BRICK_WIDTH;
            bricks[i][j].rect.height = BRICK_HEIGHT;
            bricks[i][j].active = true;
        }
    }
}

void SpawnPowerUp(PowerUp powerUps[], Vector2 position) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUps[i].active) {
            powerUps[i].position = position;
            powerUps[i].active = true;
            powerUps[i].type = 1; // Extra life
            break;
        }
    }
}

void ResetBall(Ball *ball, Paddle paddle) {
    ball->position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    ball->velocity = (Vector2){ 500.0f, -500.0f };
}
