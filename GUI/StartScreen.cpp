#include "StartScreen.h"
#include "ScreenManager.h"
#include <iostream>

StartScreen::StartScreen(ScreenManager* mgr)
    : manager(mgr),
      customerButtonHovered(false),
      staffButtonHovered(false) {
    InitializeUI();
}

StartScreen::~StartScreen() {}

void StartScreen::InitializeUI() {
    const int screenWidth  = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Button dimensions
    const int buttonWidth  = 300;
    const int buttonHeight = 80;
    const int buttonSpacing = 40;

    // Calculate button positions (centered)
    const int totalHeight = (buttonHeight * 2) + buttonSpacing;
    const int startY = (screenHeight - totalHeight) / 2;

    customerButton = Rectangle{
        (float)((screenWidth - buttonWidth) / 2),
        (float)startY,
        (float)buttonWidth,
        (float)buttonHeight
    };

    staffButton = Rectangle{
        (float)((screenWidth - buttonWidth) / 2),
        (float)(startY + buttonHeight + buttonSpacing),
        (float)buttonWidth,
        (float)buttonHeight
    };

    // Pastel plant nursery colors
    customerButtonColor = Color{206, 237, 223, 255}; // Soft mint
    staffButtonColor    = Color{230, 224, 237, 255}; // Soft lavender
}

void StartScreen::Update() {
    UpdateButtons();

    // ESC to quit
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}

void StartScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();

    // Hover
    customerButtonHovered = CheckCollisionPointRec(mousePos, customerButton);
    staffButtonHovered    = CheckCollisionPointRec(mousePos, staffButton);

    // Clicks
    if (customerButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        std::cout << "[StartScreen] Customer Mode selected\n";
        double randomBudget = 200.0 + (std::rand() % 801); // 200–1000
        manager->CreateNewCustomer(randomBudget);
        manager->SwitchScreen(GameScreen::SALES_FLOOR);
    }

    if (staffButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        std::cout << "[StartScreen] Staff Mode selected\n";
        manager->SwitchScreen(GameScreen::STAFF_GREENHOUSE);
    }
}

void StartScreen::Draw() {
    DrawBackground();
    DrawTitle();
    DrawButtons();

    // Footer hint: centered near the bottom
    const char* hint = "Press ESC to exit";
    const int fontSize = 26;
    int w = MeasureText(hint, fontSize);
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawText(hint,
             (sw - w) / 2,
             sh - 40,                       // 40 px from bottom
             fontSize,
             Color{120, 140, 125, 255});    // Medium sage
}

void StartScreen::DrawBackground() {
    ClearBackground(Color{216, 228, 220, 255}); // Soft sage green background
}

void StartScreen::DrawTitle() {
    const char* title = "PLANT NURSERY SHOP";
    const int titleFontSize = 60;
    const int titleWidth = MeasureText(title, titleFontSize);
    const int sw = GetScreenWidth();
    const int sh = GetScreenHeight();

    // Shadow
    DrawText(title,
             (sw - titleWidth) / 2 + 3,
             sh / 4 + 3,
             titleFontSize,
             Color{85, 107, 95, 100});

    // Foreground
    DrawText(title,
             (sw - titleWidth) / 2,
             sh / 4,
             titleFontSize,
             Color{85, 107, 95, 255});

    // Subtitle
    const char* subtitle = "Select Your Mode";
    const int subtitleFontSize = 24;
    const int subtitleWidth = MeasureText(subtitle, subtitleFontSize);
    DrawText(subtitle,
             (sw - subtitleWidth) / 2,
             sh / 4 + 80,
             subtitleFontSize,
             Color{120, 140, 125, 255});
}

void StartScreen::DrawButtons() {
    Color border = Color{85, 107, 95, 255}; // Dark forest green

    // Customer button
    Color custFill = customerButtonHovered ? Color{192, 230, 215, 255} : customerButtonColor;
    DrawRectangleRec(customerButton, custFill);
    DrawRectangleLinesEx(customerButton, 3, border);

    const char* customerText = "CUSTOMER MODE";
    const int customerTextSize = 30;
    int customerTextWidth = MeasureText(customerText, customerTextSize);
    DrawText(customerText,
             customerButton.x + (customerButton.width - customerTextWidth) / 2,
             customerButton.y + (customerButton.height - customerTextSize) / 2,
             customerTextSize,
             border);

    // Staff button
    Color staffFill = staffButtonHovered ? Color{218, 212, 230, 255} : staffButtonColor;
    DrawRectangleRec(staffButton, staffFill);
    DrawRectangleLinesEx(staffButton, 3, border);

    const char* staffText = "STAFF MODE";
    const int staffTextSize = 30;
    int staffTextWidth = MeasureText(staffText, staffTextSize);
    DrawText(staffText,
             staffButton.x + (staffButton.width - staffTextWidth) / 2,
             staffButton.y + (staffButton.height - staffTextSize) / 2,
             staffTextSize,
             border);
}
