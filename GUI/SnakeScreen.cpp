#include "SnakeScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

SnakeScreen::SnakeScreen(ScreenManager* mgr)
    : manager(mgr), currentDirection(Direction::RIGHT), nextDirection(Direction::RIGHT),
      gameOver(false), gameStarted(false), score(0), highScore(0), moveTimer(0.0f) {

    // Seed random
    srand(time(NULL));

    // Initialize plant types
    plantTypes = {"Rose", "Daisy", "Cactus", "Aloe", "Potato",
                  "Strelitzia", "Radish", "Monstera", "Venus Fly Trap", "Carrot"};

    InitializeGame();
}

SnakeScreen::~SnakeScreen() {}

void SnakeScreen::InitializeGame() {
    // Clear snake
    snake.clear();

    // Start snake in middle with 3 segments
    snake.push_back(Position(5, 5));
    snake.push_back(Position(4, 5));
    snake.push_back(Position(3, 5));

    currentDirection = Direction::RIGHT;
    nextDirection = Direction::RIGHT;
    gameOver = false;
    gameStarted = false;
    score = 0;
    moveTimer = 0.0f;

    SpawnFood();

    std::cout << "[Snake] Game initialized - 10x10 grid" << std::endl;
}

std::string SnakeScreen::GetRandomPlant() {
    int index = rand() % plantTypes.size();
    return plantTypes[index];
}

bool SnakeScreen::IsPositionOnSnake(const Position& pos) {
    for (const Position& segment : snake) {
        if (segment == pos) {
            return true;
        }
    }
    return false;
}

void SnakeScreen::SpawnFood() {
    // Build list of all valid positions (not occupied by snake)
    std::vector<Position> validPositions;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            Position pos(x, y);
            if (!IsPositionOnSnake(pos)) {
                validPositions.push_back(pos);
            }
        }
    }

    // If no valid positions (snake fills entire grid - shouldn't happen with 10x10)
    if (validPositions.empty()) {
        std::cout << "[Snake] ERROR: No valid positions for food!" << std::endl;
        food = Position(0, 0);
        currentFoodPlant = "Rose";
        return;
    }

    // Pick random valid position
    int index = rand() % validPositions.size();
    food = validPositions[index];
    currentFoodPlant = GetRandomPlant();

    std::cout << "[Snake] Spawned " << currentFoodPlant << " at (" << food.x << ", " << food.y << ")" << std::endl;
}

void SnakeScreen::HandleInput() {
    // Q to exit
    if (IsKeyPressed(KEY_Q)) {
        manager->SwitchScreen(GameScreen::CHEAT_MENU);
        return;
    }

    // Space to restart
    if (gameOver && IsKeyPressed(KEY_SPACE)) {
        InitializeGame();
        return;
    }

    // Arrow keys (can't reverse direction)
    if (IsKeyPressed(KEY_UP) && currentDirection != Direction::DOWN) {
        nextDirection = Direction::UP;
        gameStarted = true;
    }
    if (IsKeyPressed(KEY_DOWN) && currentDirection != Direction::UP) {
        nextDirection = Direction::DOWN;
        gameStarted = true;
    }
    if (IsKeyPressed(KEY_LEFT) && currentDirection != Direction::RIGHT) {
        nextDirection = Direction::LEFT;
        gameStarted = true;
    }
    if (IsKeyPressed(KEY_RIGHT) && currentDirection != Direction::LEFT) {
        nextDirection = Direction::RIGHT;
        gameStarted = true;
    }
}

void SnakeScreen::MoveSnake() {
    // Update direction
    currentDirection = nextDirection;

    // Calculate new head position
    Position newHead = snake[0];

    switch (currentDirection) {
        case Direction::UP:    newHead.y--; break;
        case Direction::DOWN:  newHead.y++; break;
        case Direction::LEFT:  newHead.x--; break;
        case Direction::RIGHT: newHead.x++; break;
    }

    // Add new head
    snake.insert(snake.begin(), newHead);

    // Check if ate food
    if (newHead == food) {
        score++;
        std::cout << "[Snake] Food eaten! Score: " << score << std::endl;
        SpawnFood();
        // Don't remove tail - snake grows
    } else {
        // Remove tail - snake stays same length
        snake.pop_back();
    }
}

void SnakeScreen::CheckCollisions() {
    const Position& head = snake[0];

    // Wall collision
    if (head.x < 0 || head.x >= GRID_WIDTH || head.y < 0 || head.y >= GRID_HEIGHT) {
        gameOver = true;
        if (score > highScore) highScore = score;
        std::cout << "[Snake] Wall collision! Game Over" << std::endl;
        return;
    }

    // Self collision (check if head hit body)
    for (size_t i = 1; i < snake.size(); i++) {
        if (head == snake[i]) {
            gameOver = true;
            if (score > highScore) highScore = score;
            std::cout << "[Snake] Self collision! Game Over" << std::endl;
            return;
        }
    }
}

void SnakeScreen::UpdateGame() {
    if (!gameStarted || gameOver) return;

    moveTimer += GetFrameTime();

    if (moveTimer >= MOVE_INTERVAL) {
        moveTimer = 0.0f;
        MoveSnake();
        CheckCollisions();
    }
}

void SnakeScreen::Update() {
    HandleInput();
    UpdateGame();
}

void SnakeScreen::DrawGame() {
    // Clear background
    ClearBackground(Color{20, 20, 25, 255});

    // Calculate grid offset to center it
    int gridPixelWidth = GRID_WIDTH * CELL_SIZE;
    int gridPixelHeight = GRID_HEIGHT * CELL_SIZE;
    int offsetX = (GetScreenWidth() - gridPixelWidth) / 2;
    int offsetY = (GetScreenHeight() - gridPixelHeight) / 2;

    // Draw grid background
    DrawRectangle(offsetX, offsetY, gridPixelWidth, gridPixelHeight, Color{30, 30, 35, 255});

    // Draw grid lines
    for (int x = 0; x <= GRID_WIDTH; x++) {
        DrawLine(offsetX + x * CELL_SIZE, offsetY,
                 offsetX + x * CELL_SIZE, offsetY + gridPixelHeight,
                 Color{50, 50, 55, 255});
    }
    for (int y = 0; y <= GRID_HEIGHT; y++) {
        DrawLine(offsetX, offsetY + y * CELL_SIZE,
                 offsetX + gridPixelWidth, offsetY + y * CELL_SIZE,
                 Color{50, 50, 55, 255});
    }

    // Draw border
    DrawRectangleLines(offsetX, offsetY, gridPixelWidth, gridPixelHeight, WHITE);

    // Draw food
    Texture2D foodTexture = manager->GetPlantTexture(currentFoodPlant);
    int foodPixelX = offsetX + food.x * CELL_SIZE;
    int foodPixelY = offsetY + food.y * CELL_SIZE;

    if (foodTexture.id != 0) {
        Rectangle source = {0, 0, (float)foodTexture.width, (float)foodTexture.height};
        Rectangle dest = {(float)foodPixelX + 2, (float)foodPixelY + 2,
                         (float)CELL_SIZE - 4, (float)CELL_SIZE - 4};
        DrawTexturePro(foodTexture, source, dest, Vector2{0, 0}, 0, WHITE);
    } else {
        DrawRectangle(foodPixelX + 5, foodPixelY + 5, CELL_SIZE - 10, CELL_SIZE - 10, RED);
    }

    // Draw snake
    for (size_t i = 0; i < snake.size(); i++) {
        Color segmentColor = (i == 0) ? Color{50, 205, 50, 255} : Color{34, 139, 34, 255};
        int snakePixelX = offsetX + snake[i].x * CELL_SIZE;
        int snakePixelY = offsetY + snake[i].y * CELL_SIZE;
        DrawRectangle(snakePixelX + 2, snakePixelY + 2,
                     CELL_SIZE - 4, CELL_SIZE - 4, segmentColor);
    }

    // Draw UI
    DrawText(TextFormat("Score: %d", score), 20, 20, 30, WHITE);
    DrawText(TextFormat("High Score: %d", highScore), 20, 55, 20, LIGHTGRAY);
    DrawText(TextFormat("Food: %s", currentFoodPlant.c_str()), 20, 85, 18, YELLOW);
    DrawText("Q - Return to Menu", GetScreenWidth() - 220, 20, 16, LIGHTGRAY);

    if (!gameStarted) {
        const char* text = "Press ARROW KEYS to start";
        int textWidth = MeasureText(text, 30);
        DrawText(text, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 100, 30, WHITE);
    }

    if (gameOver) {
        const char* gameOverText = "GAME OVER!";
        int width1 = MeasureText(gameOverText, 50);
        DrawText(gameOverText, GetScreenWidth() / 2 - width1 / 2, GetScreenHeight() / 2 - 50, 50, RED);

        const char* restartText = "Press SPACE to restart";
        int width2 = MeasureText(restartText, 25);
        DrawText(restartText, GetScreenWidth() / 2 - width2 / 2, GetScreenHeight() / 2 + 20, 25, WHITE);
    }
}

void SnakeScreen::Draw() {
    DrawGame();
}
