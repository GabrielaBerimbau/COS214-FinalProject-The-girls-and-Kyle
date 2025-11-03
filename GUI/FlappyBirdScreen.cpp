#include "FlappyBirdScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <algorithm>

FlappyBirdScreen::FlappyBirdScreen(ScreenManager* mgr)
    : manager(mgr), birdY(0), birdVelocity(0), gameOver(false),
      gameStarted(false), score(0), highScore(0), pipeSpeed(2.0f), pipeSpawnTimer(0) {
    InitializeGame();
}

FlappyBirdScreen::~FlappyBirdScreen() {
}

void FlappyBirdScreen::InitializeGame() {
    int screenHeight = GetScreenHeight();
    birdY = screenHeight / 2.0f;
    birdVelocity = 0;
    gameOver = false;
    gameStarted = false;
    score = 0;
    pipeSpawnTimer = 0;
    pipes.clear();
}

void FlappyBirdScreen::ResetGame() {
    InitializeGame();
}

void FlappyBirdScreen::HandleInput() {
    // Q to exit back to cheat menu
    if (IsKeyPressed(KEY_Q)) {
        manager->SwitchScreen(GameScreen::CHEAT_MENU);
        return;
    }

    // Space to flap
    if (IsKeyPressed(KEY_SPACE)) {
        if (!gameStarted) {
            gameStarted = true;
        }

        if (!gameOver) {
            birdVelocity = flapStrength;
        } else {
            // Reset game on space after game over
            ResetGame();
        }
    }
}

void FlappyBirdScreen::UpdateBird() {
    if (!gameStarted || gameOver) return;

    // Apply gravity
    birdVelocity += gravity;
    birdY += birdVelocity;

    // Check bounds
    if (birdY < 0) {
        birdY = 0;
        birdVelocity = 0;
    }

    if (birdY + birdSize > GetScreenHeight()) {
        gameOver = true;
        if (score > highScore) {
            highScore = score;
        }
    }
}

void FlappyBirdScreen::UpdatePipes() {
    if (!gameStarted || gameOver) return;

    // Spawn pipes
    pipeSpawnTimer += GetFrameTime();
    if (pipeSpawnTimer >= pipeSpawnInterval) {
        SpawnPipe();
        pipeSpawnTimer = 0;
    }

    // Move and update pipes
    for (auto it = pipes.begin(); it != pipes.end();) {
        it->x -= pipeSpeed;

        // Check if bird passed pipe (score)
        if (!it->scored && it->x + pipeWidth < birdX) {
            it->scored = true;
            score++;
        }

        // Remove off-screen pipes
        if (it->x + pipeWidth < 0) {
            it = pipes.erase(it);
        } else {
            ++it;
        }
    }
}

void FlappyBirdScreen::SpawnPipe() {
    int screenHeight = GetScreenHeight();
    int screenWidth = GetScreenWidth();

    // Random gap position (keeping it in reasonable bounds)
    float minGapY = 100;
    float maxGapY = screenHeight - pipeGapHeight - 100;
    float gapY = minGapY + (rand() % (int)(maxGapY - minGapY));

    pipes.push_back(Pipe(screenWidth, gapY, pipeGapHeight));
}

void FlappyBirdScreen::CheckCollisions() {
    if (!gameStarted || gameOver) return;

    // Get texture to calculate proper hitbox dimensions
    Texture2D birdTexture = manager->GetPlantTexture("Strelitzia");
    float aspectRatio = 1.0f;
    if (birdTexture.id != 0) {
        aspectRatio = (float)birdTexture.width / (float)birdTexture.height;
    }
    float drawWidth = birdSize * aspectRatio;
    float drawHeight = birdSize;

    // Bird hitbox (centered, slightly smaller for more forgiving gameplay)
    float hitboxPadding = 5.0f;
    Rectangle birdRect = {
        birdX - drawWidth / 2 + hitboxPadding,
        birdY - drawHeight / 2 + hitboxPadding,
        drawWidth - hitboxPadding * 2,
        drawHeight - hitboxPadding * 2
    };

    for (const auto& pipe : pipes) {
        // Top pipe
        Rectangle topPipe = {pipe.x, 0, pipeWidth, pipe.gapY};
        // Bottom pipe
        Rectangle bottomPipe = {pipe.x, pipe.gapY + pipe.gapHeight, pipeWidth, (float)GetScreenHeight()};

        if (CheckCollisionRecs(birdRect, topPipe) || CheckCollisionRecs(birdRect, bottomPipe)) {
            gameOver = true;
            if (score > highScore) {
                highScore = score;
            }
        }
    }
}

void FlappyBirdScreen::Update() {
    HandleInput();
    UpdateBird();
    UpdatePipes();
    CheckCollisions();
}

void FlappyBirdScreen::DrawBird() {
    // Get Strelitzia texture from manager
    Texture2D birdTexture = manager->GetPlantTexture("Strelitzia");

    if (birdTexture.id != 0) {
        // Calculate aspect ratio to maintain proportions
        float aspectRatio = (float)birdTexture.width / (float)birdTexture.height;
        float drawWidth = birdSize * aspectRatio;
        float drawHeight = birdSize;

        // Debug output (only print once at start)
        static bool debugPrinted = false;
        if (!debugPrinted) {
            std::cout << "[FlappyBird] Texture size: " << birdTexture.width << "x" << birdTexture.height << std::endl;
            std::cout << "[FlappyBird] Draw size: " << drawWidth << "x" << drawHeight << std::endl;
            std::cout << "[FlappyBird] birdSize: " << birdSize << std::endl;
            debugPrinted = true;
        }

        // Scale texture maintaining aspect ratio
        Rectangle source = {0, 0, (float)birdTexture.width, (float)birdTexture.height};
        Rectangle dest = {birdX, birdY, drawWidth, drawHeight};
        Vector2 origin = {drawWidth / 2, drawHeight / 2}; // Center rotation
        float rotation = birdVelocity * 3; // Slight rotation based on velocity

        DrawTexturePro(birdTexture, source, dest, origin, rotation, WHITE);
    } else {
        // Fallback: draw a simple circle
        std::cout << "[FlappyBird] WARNING: Strelitzia texture not loaded!" << std::endl;
        DrawCircle(birdX, birdY, birdSize / 2, ORANGE);
    }
}

void FlappyBirdScreen::DrawPipes() {
    for (const auto& pipe : pipes) {
        // Top pipe
        DrawRectangle(pipe.x, 0, pipeWidth, pipe.gapY, pipeGreen);
        // Bottom pipe
        DrawRectangle(pipe.x, pipe.gapY + pipe.gapHeight, pipeWidth,
                      GetScreenHeight() - (pipe.gapY + pipe.gapHeight), pipeGreen);

        // Pipe borders for definition
        DrawRectangleLines(pipe.x, 0, pipeWidth, pipe.gapY, DARKGREEN);
        DrawRectangleLines(pipe.x, pipe.gapY + pipe.gapHeight, pipeWidth,
                           GetScreenHeight() - (pipe.gapY + pipe.gapHeight), DARKGREEN);
    }
}

void FlappyBirdScreen::DrawUI() {
    int screenWidth = GetScreenWidth();

    // Score
    std::string scoreText = "Score: " + std::to_string(score);
    DrawText(scoreText.c_str(), 20, 20, 30, BLACK);

    // High score
    std::string highScoreText = "High Score: " + std::to_string(highScore);
    DrawText(highScoreText.c_str(), 20, 60, 20, DARKGRAY);

    // Instructions
    if (!gameStarted) {
        const char* startText = "Press SPACE to start";
        int textWidth = MeasureText(startText, 40);
        DrawText(startText, screenWidth / 2 - textWidth / 2, GetScreenHeight() / 2 - 50, 40, BLACK);
    }

    if (gameOver) {
        const char* gameOverText = "GAME OVER!";
        int textWidth = MeasureText(gameOverText, 50);
        DrawText(gameOverText, screenWidth / 2 - textWidth / 2, GetScreenHeight() / 2 - 100, 50, RED);

        const char* restartText = "Press SPACE to restart or Q to exit";
        int restartWidth = MeasureText(restartText, 20);
        DrawText(restartText, screenWidth / 2 - restartWidth / 2, GetScreenHeight() / 2, 20, BLACK);
    }

    // Q hint
    const char* quitText = "Q - Return to Menu";
    DrawText(quitText, screenWidth - 200, 20, 15, DARKGRAY);
}

void FlappyBirdScreen::Draw() {
    ClearBackground(skyBlue);

    DrawPipes();
    DrawBird();
    DrawUI();
}
