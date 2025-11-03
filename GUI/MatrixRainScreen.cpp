#include "MatrixRainScreen.h"
#include "ScreenManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

MatrixRainScreen::MatrixRainScreen(ScreenManager* mgr)
    : manager(mgr) {
    std::srand(std::time(nullptr));
    InitializeColumns();
}

MatrixRainScreen::~MatrixRainScreen() {
}

void MatrixRainScreen::InitializeColumns() {
    columns.clear();
    columns.resize(NUM_COLUMNS);

    for (int i = 0; i < NUM_COLUMNS; i++) {
        ResetColumn(i);
        // Stagger initial positions
        columns[i].yPosition = -(std::rand() % 900);
    }
}

void MatrixRainScreen::ResetColumn(int columnIndex) {
    if (columnIndex < 0 || columnIndex >= (int)columns.size()) return;

    MatrixColumn& col = columns[columnIndex];

    // Random starting position above screen
    col.yPosition = -(std::rand() % 500 + 100);

    // Random speed variation
    col.speed = BASE_FALL_SPEED + (std::rand() % 100 - 50);

    // Random length (10-30 characters)
    col.length = std::rand() % 21 + 10;

    // Generate random characters
    col.characters.clear();
    for (int i = 0; i < col.length; i++) {
        col.characters += GetRandomCharacter();
    }

    // Bright head
    col.headBrightness = 1.0f;
}

char MatrixRainScreen::GetRandomCharacter() {
    // Mix of katakana-like characters, numbers, and symbols
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@#$%&*+-=<>[]{}|";
    return charset[std::rand() % (sizeof(charset) - 1)];
}

void MatrixRainScreen::Update() {
    // Q to exit back to cheat menu
    if (IsKeyPressed(KEY_Q)) {
        manager->SwitchScreen(GameScreen::CHEAT_MENU);
        return;
    }

    // Update all columns
    float deltaTime = GetFrameTime();

    for (int i = 0; i < NUM_COLUMNS; i++) {
        MatrixColumn& col = columns[i];

        // Move column down
        col.yPosition += col.speed * deltaTime;

        // Randomly change characters occasionally
        if (std::rand() % 100 < 5) {
            int charIndex = std::rand() % col.length;
            col.characters[charIndex] = GetRandomCharacter();
        }

        // Reset column if it's completely off screen
        if (col.yPosition > GetScreenHeight() + (col.length * COLUMN_WIDTH)) {
            ResetColumn(i);
        }
    }
}

void MatrixRainScreen::Draw() {
    ClearBackground(matrixBlack);

    // Draw all columns
    for (int i = 0; i < NUM_COLUMNS; i++) {
        const MatrixColumn& col = columns[i];
        int xPos = i * COLUMN_WIDTH;

        // Draw each character in the column
        for (int j = 0; j < col.length; j++) {
            int yPos = (int)(col.yPosition + (j * COLUMN_WIDTH));

            // Skip if off screen
            if (yPos < -COLUMN_WIDTH || yPos > GetScreenHeight()) continue;

            // Calculate brightness based on position in column
            float brightness;
            if (j == 0) {
                // Head of the column - brightest (white)
                brightness = 1.0f;
            } else if (j < 3) {
                // Near head - bright green
                brightness = 0.8f - (j * 0.2f);
            } else {
                // Tail - fade to dark
                brightness = 0.4f - ((float)(j - 3) / col.length * 0.4f);
            }

            // Clamp brightness
            if (brightness < 0.1f) brightness = 0.1f;

            Color charColor;
            if (j == 0) {
                // Head is white
                charColor = Color{
                    (unsigned char)(255 * brightness),
                    (unsigned char)(255 * brightness),
                    (unsigned char)(255 * brightness),
                    255
                };
            } else {
                // Rest is green
                charColor = Color{
                    0,
                    (unsigned char)(255 * brightness),
                    (unsigned char)(65 * brightness),
                    255
                };
            }

            // Draw the character
            DrawText(
                &col.characters[j],
                xPos,
                yPos,
                COLUMN_WIDTH,
                charColor
            );
        }
    }

    // Draw instructions at bottom
    const char* instructions = "Q - Back to Cheat Menu";
    int instrWidth = MeasureText(instructions, 20);
    DrawText(instructions, GetScreenWidth() / 2 - instrWidth / 2, GetScreenHeight() - 40, 20, matrixGreen);

    // Draw title at top (fading in and out)
    static float titleAlpha = 0.0f;
    static float titleDirection = 1.0f;
    titleAlpha += titleDirection * GetFrameTime() * 0.5f;
    if (titleAlpha >= 1.0f) {
        titleAlpha = 1.0f;
        titleDirection = -1.0f;
    } else if (titleAlpha <= 0.3f) {
        titleAlpha = 0.3f;
        titleDirection = 1.0f;
    }

    const char* title = "MATRIX RAIN";
    int titleWidth = MeasureText(title, 40);
    Color titleColor = matrixGreen;
    titleColor.a = (unsigned char)(255 * titleAlpha);
    DrawText(title, GetScreenWidth() / 2 - titleWidth / 2, 30, 40, titleColor);
}
