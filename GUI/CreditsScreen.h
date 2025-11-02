#ifndef CREDITS_SCREEN_H
#define CREDITS_SCREEN_H

#include "raylib.h"
#include <string>
#include <vector>

class ScreenManager;

/**
 * @class CreditsScreen
 * @brief Credits/about screen with terminal aesthetic
 *
 * Displays project information, team members, and design patterns used
 * Matching terminal style with CheatMenuScreen
 */
class CreditsScreen {
private:
    ScreenManager* manager;

    // Terminal aesthetic
    const Color terminalGreen = {0, 255, 0, 255};
    const Color terminalBlack = {0, 0, 0, 255};

    // Scroll offset for long credits
    float scrollOffset;

    std::vector<std::string> creditLines;

    void InitializeCredits();
    void HandleInput();
    void DrawTerminalBackground();
    void DrawCredits();

public:
    CreditsScreen(ScreenManager* mgr);
    ~CreditsScreen();

    void Update();
    void Draw();
};

#endif // CREDITS_SCREEN_H
