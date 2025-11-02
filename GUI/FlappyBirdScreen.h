#ifndef FLAPPY_BIRD_SCREEN_H
#define FLAPPY_BIRD_SCREEN_H

#include "raylib.h"
#include <vector>

class ScreenManager;

/**
 * @struct Pipe
 * @brief Represents an obstacle pipe in Flappy Bird
 */
struct Pipe {
    float x;
    float gapY;
    float gapHeight;
    bool scored;

    Pipe(float xPos, float y, float height)
        : x(xPos), gapY(y), gapHeight(height), scored(false) {}
};

/**
 * @class FlappyBirdScreen
 * @brief Simple Flappy Bird minigame using Strelitzia sprite
 *
 * Easter egg minigame - press SPACE to flap, ESC to exit
 * Session-based high score tracking
 */
class FlappyBirdScreen {
private:
    ScreenManager* manager;

    // Bird properties
    float birdY;
    float birdVelocity;
    const float birdX = 100.0f;
    const float gravity = 0.25f;        // Reduced from 0.5f - slower fall
    const float flapStrength = -5.0f;   // Reduced from -8.0f - gentler flap
    const float birdSize = 100.0f;       // Increased from 40.0f - bigger plant sprite

    // Game state
    bool gameOver;
    bool gameStarted;
    int score;
    int highScore;

    // Pipes
    std::vector<Pipe> pipes;
    float pipeSpeed;
    float pipeSpawnTimer;
    const float pipeSpawnInterval = 2.0f;
    const float pipeWidth = 100.0f;
    const float pipeGapHeight = 180.0f; 

    // Colors
    const Color skyBlue = {135, 206, 235, 255};
    const Color pipeGreen = {34, 139, 34, 255};

    void InitializeGame();
    void HandleInput();
    void UpdateBird();
    void UpdatePipes();
    void SpawnPipe();
    void CheckCollisions();
    void DrawBird();
    void DrawPipes();
    void DrawUI();
    void ResetGame();

public:
    FlappyBirdScreen(ScreenManager* mgr);
    ~FlappyBirdScreen();

    void Update();
    void Draw();
};

#endif // FLAPPY_BIRD_SCREEN_H
