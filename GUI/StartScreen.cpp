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

    // Colors are now set dynamically in Draw methods based on color scheme
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
    const char* hint = "Press ESC to exit | Press X to toggle color scheme";
    const int fontSize = 20;
    int w = MeasureText(hint, fontSize);
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    Color hintColor = manager->IsAlternativeColors()
        ? Color{160, 160, 160, 255}  // Light grey for dark mode
        : Color{120, 140, 125, 255}; // Medium sage

    DrawText(hint,
             (sw - w) / 2,
             sh - 40,                       // 40 px from bottom
             fontSize,
             hintColor);
}

void StartScreen::DrawBackground() {
    // Light mode: Soft sage green | Dark mode: Dark background
    Color bgColor = manager->IsAlternativeColors()
        ? Color{30, 30, 35, 255}     // Dark mode background
        : Color{216, 228, 220, 255}; // Light sage green
    ClearBackground(bgColor);
}

void StartScreen::DrawTitle() {
    const char* title = "PLANT NURSERY SHOP";
    const int titleFontSize = 60;
    const int titleWidth = MeasureText(title, titleFontSize);
    const int sw = GetScreenWidth();
    const int sh = GetScreenHeight();

    // Light mode: Dark forest green | Dark mode: Light text
    Color titleShadow = manager->IsAlternativeColors()
        ? Color{0, 0, 0, 120}        // Black shadow for dark mode
        : Color{85, 107, 95, 100};   // Dark forest green shadow

    Color titleColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint green for dark mode
        : Color{85, 107, 95, 255};   // Dark forest green

    Color subtitleColor = manager->IsAlternativeColors()
        ? Color{180, 200, 190, 255}  // Light grey-green for dark mode
        : Color{120, 140, 125, 255}; // Medium sage

    // Shadow
    DrawText(title,
             (sw - titleWidth) / 2 + 3,
             sh / 4 + 3,
             titleFontSize,
             titleShadow);

    // Foreground
    DrawText(title,
             (sw - titleWidth) / 2,
             sh / 4,
             titleFontSize,
             titleColor);

    // Subtitle
    const char* subtitle = "Select Your Mode";
    const int subtitleFontSize = 24;
    const int subtitleWidth = MeasureText(subtitle, subtitleFontSize);
    DrawText(subtitle,
             (sw - subtitleWidth) / 2,
             sh / 4 + 80,
             subtitleFontSize,
             subtitleColor);
}

void StartScreen::DrawButtons() {
    // Light mode: Dark forest green | Dark mode: Light border
    Color border = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint green border
        : Color{85, 107, 95, 255};   // Dark forest green

    // Button text color
    Color textColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text for dark mode
        : Color{85, 107, 95, 255};   // Dark text for light mode

    // Customer button - Light mode: Soft mint | Dark mode: Dark blue-green
    Color custFillIdle = manager->IsAlternativeColors()
        ? Color{45, 65, 60, 255}     // Dark blue-green
        : Color{206, 237, 223, 255}; // Soft mint
    Color custFillHover = manager->IsAlternativeColors()
        ? Color{55, 80, 70, 255}     // Lighter on hover
        : Color{192, 230, 215, 255}; // Darker mint hover
    Color custFill = customerButtonHovered ? custFillHover : custFillIdle;
    DrawRectangleRec(customerButton, custFill);
    DrawRectangleLinesEx(customerButton, 3, border);

    const char* customerText = "CUSTOMER MODE";
    const int customerTextSize = 30;
    int customerTextWidth = MeasureText(customerText, customerTextSize);
    DrawText(customerText,
             customerButton.x + (customerButton.width - customerTextWidth) / 2,
             customerButton.y + (customerButton.height - customerTextSize) / 2,
             customerTextSize,
             textColor);

    // Staff button - Light mode: Soft lavender | Dark mode: Dark purple
    Color staffFillIdle = manager->IsAlternativeColors()
        ? Color{55, 45, 65, 255}     // Dark purple
        : Color{230, 224, 237, 255}; // Soft lavender
    Color staffFillHover = manager->IsAlternativeColors()
        ? Color{70, 55, 85, 255}     // Lighter on hover
        : Color{218, 212, 230, 255}; // Darker lavender hover
    Color staffFill = staffButtonHovered ? staffFillHover : staffFillIdle;
    DrawRectangleRec(staffButton, staffFill);
    DrawRectangleLinesEx(staffButton, 3, border);

    const char* staffText = "STAFF MODE";
    const int staffTextSize = 30;
    int staffTextWidth = MeasureText(staffText, staffTextSize);
    DrawText(staffText,
             staffButton.x + (staffButton.width - staffTextWidth) / 2,
             staffButton.y + (staffButton.height - staffTextSize) / 2,
             staffTextSize,
             textColor);
}
