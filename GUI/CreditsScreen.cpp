#include "CreditsScreen.h"
#include "ScreenManager.h"
#include <iostream>

CreditsScreen::CreditsScreen(ScreenManager* mgr)
    : manager(mgr), scrollOffset(0) {
    InitializeCredits();
}

CreditsScreen::~CreditsScreen() {
}

void CreditsScreen::InitializeCredits() {
    creditLines.clear();

    creditLines.push_back("==============================================");
    creditLines.push_back("");
    creditLines.push_back("       PLANT NURSERY MANAGEMENT SYSTEM");
    creditLines.push_back("");
    creditLines.push_back("==============================================");
    creditLines.push_back("");
    creditLines.push_back("");
    creditLines.push_back("DEVELOPED BY:");
    creditLines.push_back("");
    creditLines.push_back("  [Gabriela, Sofia, Rachel, Kahlan, Gabi, Kyle, Megan]");
    creditLines.push_back("");
    creditLines.push_back("");
    creditLines.push_back("DESIGN PATTERNS IMPLEMENTED:");
    creditLines.push_back("");
    creditLines.push_back("  - Factory Pattern");
    creditLines.push_back("  - Singleton Pattern");
    creditLines.push_back("  - Observer Pattern");
    creditLines.push_back("  - Strategy Pattern");
    creditLines.push_back("  - State Pattern");
    creditLines.push_back("  - Decorator Pattern");
    creditLines.push_back("  - Composite Pattern");
    creditLines.push_back("  - Iterator Pattern");
    creditLines.push_back("  - Chain of Responsibility");
    creditLines.push_back("  - Mediator Pattern");
    creditLines.push_back("  - Prototype Pattern");
    creditLines.push_back("  - Template Method Pattern");
    creditLines.push_back("");
    creditLines.push_back("");
    creditLines.push_back("  Thanks for finding the secret menu!");
    creditLines.push_back("  Konami Code: UP UP DOWN DOWN LEFT RIGHT LEFT RIGHT B A");
    creditLines.push_back("");
    creditLines.push_back("");
    creditLines.push_back("BUILT WITH:");
    creditLines.push_back("");
    creditLines.push_back("  - C++");
    creditLines.push_back("  - Raylib");
    creditLines.push_back("  - GoogleTest");
    creditLines.push_back("");
    creditLines.push_back("");
    creditLines.push_back("==============================================");
    creditLines.push_back("");
    creditLines.push_back("         Press Q to return to menu");
    creditLines.push_back("");
    creditLines.push_back("==============================================");
}

void CreditsScreen::HandleInput() {
    // Q to return to cheat menu
    if (IsKeyPressed(KEY_Q)) {
        manager->SwitchScreen(GameScreen::CHEAT_MENU);
    }

    // Arrow keys to scroll
    if (IsKeyDown(KEY_UP)) {
        scrollOffset -= 100 * GetFrameTime();
        if (scrollOffset < 0) scrollOffset = 0;
    }

    if (IsKeyDown(KEY_DOWN)) {
        scrollOffset += 100 * GetFrameTime();
        float maxScroll = creditLines.size() * 25.0f - GetScreenHeight() + 100;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
        if (scrollOffset < 0) scrollOffset = 0;
    }
}

void CreditsScreen::DrawTerminalBackground() {
    ClearBackground(terminalBlack);

    // Draw scanlines for CRT effect
    for (int y = 0; y < GetScreenHeight(); y += 4) {
        DrawLine(0, y, GetScreenWidth(), y, Color{0, 20, 0, 50});
    }
}

void CreditsScreen::DrawCredits() {
    int screenWidth = GetScreenWidth();
    int startY = 50 - (int)scrollOffset;

    for (size_t i = 0; i < creditLines.size(); i++) {
        int y = startY + (i * 25);

        // Only draw if on screen
        if (y >= -30 && y <= GetScreenHeight()) {
            int textWidth = MeasureText(creditLines[i].c_str(), 20);
            int x = screenWidth / 2 - textWidth / 2;
            DrawText(creditLines[i].c_str(), x, y, 20, terminalGreen);
        }
    }

    // Draw scroll hints if content is scrollable
    if (creditLines.size() * 25 > (size_t)GetScreenHeight()) {
        const char* scrollHint = "USE ARROW KEYS TO SCROLL";
        int hintWidth = MeasureText(scrollHint, 15);
        DrawText(scrollHint, screenWidth / 2 - hintWidth / 2, GetScreenHeight() - 30, 15, terminalGreen);
    }
}

void CreditsScreen::Update() {
    HandleInput();
}

void CreditsScreen::Draw() {
    DrawTerminalBackground();
    DrawCredits();
}
