#include "CheatMenuScreen.h"
#include "ScreenManager.h"
#include <iostream>

CheatMenuScreen::CheatMenuScreen(ScreenManager* mgr)
    : manager(mgr), selectedOption(0), blinkTimer(0.0f), showCursor(true) {
    InitializeMenu();
}

CheatMenuScreen::~CheatMenuScreen() {
}

void CheatMenuScreen::InitializeMenu() {
    menuOptions.clear();
    menuOptions.push_back("FLAPPY BIRD");
    menuOptions.push_back("SNAKE");
    menuOptions.push_back("MATRIX RAIN");
    menuOptions.push_back("CHEATS");
    menuOptions.push_back("CREDITS");
    menuOptions.push_back("EXIT");
    selectedOption = 0;
}

void CheatMenuScreen::HandleInput() {
    // Arrow key navigation
    if (IsKeyPressed(KEY_UP)) {
        selectedOption--;
        if (selectedOption < 0) {
            selectedOption = menuOptions.size() - 1;
        }
    }

    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption++;
        if (selectedOption >= (int)menuOptions.size()) {
            selectedOption = 0;
        }
    }

    // Enter to select
    if (IsKeyPressed(KEY_ENTER)) {
        ExecuteSelectedOption();
    }

    // Q to exit
    if (IsKeyPressed(KEY_Q)) {
        manager->SwitchScreen(GameScreen::SALES_FLOOR);
    }
}

void CheatMenuScreen::ExecuteSelectedOption() {
    std::cout << "[CheatMenu] Selected option: " << menuOptions[selectedOption] << std::endl;

    if (menuOptions[selectedOption] == "FLAPPY BIRD") {
        manager->SwitchScreen(GameScreen::FLAPPY_BIRD);
    } else if (menuOptions[selectedOption] == "SNAKE") {
        manager->SwitchScreen(GameScreen::SNAKE);
    } else if (menuOptions[selectedOption] == "MATRIX RAIN") {
        manager->SwitchScreen(GameScreen::MATRIX_RAIN);
    } else if (menuOptions[selectedOption] == "CHEATS") {
        manager->SwitchScreen(GameScreen::CHEATS_SUBMENU);
    } else if (menuOptions[selectedOption] == "CREDITS") {
        manager->SwitchScreen(GameScreen::CREDITS);
    } else if (menuOptions[selectedOption] == "EXIT") {
        manager->SwitchScreen(GameScreen::SALES_FLOOR);
    }
}

void CheatMenuScreen::Update() {
    HandleInput();

    // Cursor blink animation
    blinkTimer += GetFrameTime();
    if (blinkTimer >= 0.5f) {
        showCursor = !showCursor;
        blinkTimer = 0.0f;
    }
}

void CheatMenuScreen::DrawTerminalBackground() {
    ClearBackground(terminalBlack);

    // Draw scanlines for CRT effect (subtle)
    for (int y = 0; y < GetScreenHeight(); y += 4) {
        DrawLine(0, y, GetScreenWidth(), y, Color{0, 20, 0, 50});
    }
}

void CheatMenuScreen::DrawMenuOptions() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Draw title
    const char* title = "=== CHEAT MENU ===";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, screenWidth / 2 - titleWidth / 2, 100, 40, terminalGreen);

    // Draw menu options
    int startY = screenHeight / 2 - (menuOptions.size() * 30);
    for (size_t i = 0; i < menuOptions.size(); i++) {
        Color color = (i == (size_t)selectedOption) ? selectedColor : terminalGreen;
        std::string prefix = (i == (size_t)selectedOption) ? "> " : "  ";
        std::string text = prefix + menuOptions[i];

        int textWidth = MeasureText(text.c_str(), 30);
        int posX = screenWidth / 2 - textWidth / 2;
        int posY = startY + (i * 50);

        DrawText(text.c_str(), posX, posY, 30, color);

        // Draw cursor
        if (i == (size_t)selectedOption && showCursor) {
            DrawText("_", posX + textWidth + 5, posY, 30, selectedColor);
        }
    }

    // Draw instructions at bottom
    const char* instructions = "USE ARROW KEYS TO NAVIGATE | ENTER TO SELECT | Q TO EXIT";
    int instrWidth = MeasureText(instructions, 15);
    DrawText(instructions, screenWidth / 2 - instrWidth / 2, screenHeight - 50, 15, terminalGreen);
}

void CheatMenuScreen::DrawCursor() {
    // Cursor is drawn in DrawMenuOptions
}

void CheatMenuScreen::Draw() {
    DrawTerminalBackground();
    DrawMenuOptions();
}
