#ifndef SNAKE_SCREEN_H
#define SNAKE_SCREEN_H

#include "raylib.h"
#include <vector>
#include <string>

class ScreenManager;

/**
 * @struct Position
 * @brief Simple 2D position for snake segments and food
 */
struct Position {
    int x;
    int y;

    Position(int xPos, int yPos) : x(xPos), y(yPos) {}
    Position() : x(0), y(0) {}

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

/**
 * @enum Direction
 * @brief Snake movement directions
 */
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @class SnakeScreen
 * @brief Simple 10x10 Snake game with random plant sprites as food
 *
 * Hardcoded 10x10 grid, guaranteed food spawning
 */
class SnakeScreen {
private:
    ScreenManager* manager;

    // Fixed 10x10 grid
    static const int GRID_WIDTH = 10;
    static const int GRID_HEIGHT = 10;
    static const int CELL_SIZE = 80;  // Pixels per cell

    // Snake
    std::vector<Position> snake;
    Direction currentDirection;
    Direction nextDirection;

    // Food
    Position food;
    std::string currentFoodPlant;

    // Game state
    bool gameOver;
    bool gameStarted;
    int score;
    int highScore;

    // Timing
    float moveTimer;
    static constexpr float MOVE_INTERVAL = 0.2f;

    // Available plant types
    std::vector<std::string> plantTypes;

    // Helper methods
    void InitializeGame();
    void HandleInput();
    void UpdateGame();
    void SpawnFood();
    bool IsPositionOnSnake(const Position& pos);
    void MoveSnake();
    void CheckCollisions();
    void DrawGame();
    std::string GetRandomPlant();

public:
    SnakeScreen(ScreenManager* mgr);
    ~SnakeScreen();

    void Update();
    void Draw();
};

#endif // SNAKE_SCREEN_H
