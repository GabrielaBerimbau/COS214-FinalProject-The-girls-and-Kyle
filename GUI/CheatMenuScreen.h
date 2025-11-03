#ifndef CHEAT_MENU_SCREEN_H
#define CHEAT_MENU_SCREEN_H

#include "raylib.h"
#include <vector>
#include <string>

class ScreenManager;

/**
 * @class CheatMenuScreen
 * @brief Terminal-style cheat menu with green text on black background
 *
 * This is an easter egg screen accessed via Konami code or typing "cheat"
 * Features retro terminal aesthetic with arrow key navigation
 */
class CheatMenuScreen {
private:
    ScreenManager* manager;

    // Menu options
    std::vector<std::string> menuOptions;
    int selectedOption;

    // Terminal aesthetic
    const Color terminalGreen = {0, 255, 0, 255};
    const Color terminalBlack = {0, 0, 0, 255};
    const Color selectedColor = {0, 200, 0, 255};

    // Cursor blink animation
    float blinkTimer;
    bool showCursor;

    void InitializeMenu();
    void HandleInput();
    void DrawTerminalBackground();
    void DrawMenuOptions();
    void DrawCursor();
    void ExecuteSelectedOption();

public:
    CheatMenuScreen(ScreenManager* mgr);
    ~CheatMenuScreen();

    void Update();
    void Draw();
};

#endif // CHEAT_MENU_SCREEN_H
