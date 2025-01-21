
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define BALL_RADIUS 10
#define BRICK_ROWS 5
#define BRICK_COLUMNS 9
#define BRICK_WIDTH 70
#define BRICK_HEIGHT 30
#define MAX_POWERUPS 5

typedef struct Ball{
  Vector2 position;
  Vector2 velocity;
  float radius;
} Ball;

typedef struct Paddle{
  Vector2 position;
  float width;
  float height;
} Paddle;

typedef struct Brick{
  Rectangle rect;
  bool active;
} Brick;

typedef struct Powerup{
  Vector2 position;
  bool active;
  int type; // 1 = extra life
} Powerup;

// Functions
void InitializeBricks(Brick bricks[BRICK_ROWS][BRICK_COLUMNS]);
void SpawnPowerUp(PowerUp powerUps[], vector2 position);
void ResetBall(Ball *ball, Paddle paddle);

int main() {
  // Game Start
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkanoider");
  SetTargetFPS(60);
  srand(time(NULL));

  //Game Objects
  Ball ball = {
    .position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
    .velocity = { 200.0f, 200.0f },
    .radius = BALL_RADIUS,
  };

  Paddle paddle = {
    .position = { (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f, SCREEN_HEIGHT - 50 },
    .width = PADDLE_WIDTH,
    .height = PADDLE_HEIGHT,
  };

  Brick bricks[BRICK_ROWS][BRICK_COLUMNS];
  InitializeBricks(bricks);

  Powerup powerups[MAX_POWERUPS] = { 0 };
  int lives = 3;
  int bricksRemaining = BRICK_ROWS * BRICK_COLUMNS;

  // Game Loop
  while (!WindowShouldClose()) {
    // Ball Movement
    ball.position.x += ball.velocity.x * GetFrameTime();
    ball.position.y += ball.velocity.y * GetFrameTime();

    // Ball Wall Collision
    if (ball.position.x <= 0 || ball.position.x >= SCREEN_WIDTH) {
      ball.velocity.x *= -1;
    }
    if (ball.postition.y <= 0) {
      ball.velocity.y *= -1;
    }

    // Paddle Collision
    Rectangle paddleRect = {paddle.position.x, paddle.position.y, paddle.width, paddle.height};
    if (CheckCollisionCircleRec(ball.position, ball.radius, paddleRect)) {
      ball.velocity.y *= -1;
      ball.position.y = paddle.position.y - ball.radius; // Makes sure Ball bounces
    }

    // loss of life / Ball dropped
    if (ball.position.y >= SCREEN_HEIGHT) {
      lives--;
      if (lives <= 0) {
        //Game over
        DrawText("Game Over", SCREEN_WIDTH / 2 - 100 , (SCREEN_HEIGHT / 2 - 20, 40, RED);
        EndDrawing();
        CloseWindow();
        return 0;
      } else {
          resetBall(&ball, paddle);
      }
    }

    // Collision With Bricks
    for (int i = 0; i < BRICK_ROWS; i++) {
      for (int j = 0; j < BRICK_COLUMNS; j++) {
        if (bricks[i][j].active && CheckCollisionCircleRec(ball.position, ball.radius, bricks[i][j].rect)) {
          bricks[i][j].active = false;
          ball.velocity.y *= -1;
          bricksRemaining--;

          // 20% chance to spawn a PowerUp
          if (rand() % 5 == 0) {
              spawnPowerUp(powerups, (vector2){bricks[i][j].rect.x + BRICK_WIDTH / 2, bricks[i][j].rect.y + BRICK_HEIGHT});
          }
          break;
        }
      }
    }

    // Paddle Movement

  }
}