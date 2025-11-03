#ifndef MATRIX_RAIN_SCREEN_H
#define MATRIX_RAIN_SCREEN_H

#include "raylib.h"
#include <vector>
#include <string>

class ScreenManager;

/**
 * @class MatrixRainScreen
 * @brief Displays the classic Matrix "digital rain" effect
 *
 * Creates falling streams of green characters reminiscent of the Matrix films.
 * Features multiple columns of characters falling at different speeds with
 * varying brightness to create depth.
 */
class MatrixRainScreen {
private:
    ScreenManager* manager;

    // Matrix rain configuration
    static constexpr int COLUMN_WIDTH = 20;
    static constexpr int NUM_COLUMNS = 70;  // 1400 / 20
    static constexpr float BASE_FALL_SPEED = 50.0f;

    // Column data structure
    struct MatrixColumn {
        float yPosition;
        float speed;
        int length;
        std::string characters;
        float headBrightness;

        MatrixColumn() : yPosition(0), speed(0), length(0), headBrightness(1.0f) {}
    };

    std::vector<MatrixColumn> columns;

    // Colors
    const Color matrixGreen = {0, 255, 65, 255};
    const Color matrixDarkGreen = {0, 100, 0, 255};
    const Color matrixBlack = {0, 0, 0, 255};

    void InitializeColumns();
    void ResetColumn(int columnIndex);
    char GetRandomCharacter();

public:
    MatrixRainScreen(ScreenManager* mgr);
    ~MatrixRainScreen();

    void Update();
    void Draw();
};

#endif // MATRIX_RAIN_SCREEN_H
