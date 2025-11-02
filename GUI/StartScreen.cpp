#include "StartScreen.h"
#include "ScreenManager.h"
#include <iostream>

StartScreen::StartScreen(ScreenManager* mgr) 
    : manager(mgr),
      customerButtonHovered(false),
      staffButtonHovered(false) {
    
    InitializeUI();
}

StartScreen::~StartScreen() {
    // Nothing to clean up
}

void StartScreen::InitializeUI() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Button dimensions
    int buttonWidth = 300;
    int buttonHeight = 80;
    int buttonSpacing = 40;
    
    // Calculate button positions (centered)
    int totalHeight = (buttonHeight * 2) + buttonSpacing;
    int startY = (screenHeight - totalHeight) / 2;
    
    customerButton = Rectangle{
        static_cast<float>((screenWidth - buttonWidth) / 2),
        static_cast<float>(startY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    staffButton = Rectangle{
        static_cast<float>((screenWidth - buttonWidth) / 2),
        static_cast<float>(startY + buttonHeight + buttonSpacing),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    // Pastel plant nursery colors
    customerButtonColor = Color{206, 237, 223, 255}; // Soft mint
    staffButtonColor = Color{230, 224, 237, 255}; // Soft lavender
}

void StartScreen::Update() {
    UpdateButtons();
}

void StartScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();
    
    // Check customer button hover
    customerButtonHovered = CheckCollisionPointRec(mousePos, customerButton);
    
    // Check staff button hover
    staffButtonHovered = CheckCollisionPointRec(mousePos, staffButton);
    
    // Handle customer button click
    if (customerButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        std::cout << "[StartScreen] Customer Mode selected" << std::endl;
        
        // Generate random budget between 200 and 1000
        double randomBudget = 200.0 + (std::rand() % 801); // 200 to 1000
        
        // Create new customer
        manager->CreateNewCustomer(randomBudget);
        
        // Switch to sales floor screen
        manager->SwitchScreen(GameScreen::SALES_FLOOR);
    }
    
    // Handle staff button click
    if (staffButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        std::cout << "[StartScreen] Staff Mode selected" << std::endl;
        
        // Switch to staff greenhouse screen
        manager->SwitchScreen(GameScreen::STAFF_GREENHOUSE);
    }
}

void StartScreen::Draw() {
    DrawBackground();
    DrawTitle();
    DrawButtons();
}

void StartScreen::DrawBackground() {
    ClearBackground(Color{216, 228, 220, 255}); // Soft sage green background
}

void StartScreen::DrawTitle() {
    const char* title = "PLANT NURSERY SHOP";
    int titleFontSize = 60;
    int titleWidth = MeasureText(title, titleFontSize);
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Draw title shadow
    DrawText(title,
             (screenWidth - titleWidth) / 2 + 3,
             screenHeight / 4 + 3,
             titleFontSize,
             Color{85, 107, 95, 100}); // Soft shadow

    // Draw title
    DrawText(title,
             (screenWidth - titleWidth) / 2,
             screenHeight / 4,
             titleFontSize,
             Color{85, 107, 95, 255}); // Dark forest green
    
    // Draw subtitle
    const char* subtitle = "Select Your Mode";
    int subtitleFontSize = 24;
    int subtitleWidth = MeasureText(subtitle, subtitleFontSize);
    DrawText(subtitle,
             (screenWidth - subtitleWidth) / 2,
             screenHeight / 4 + 80,
             subtitleFontSize,
             Color{120, 140, 125, 255}); // Medium sage green
}

void StartScreen::DrawButtons() {
    // Customer button
    Color custBtnColor = customerButtonHovered ? Color{255, 200, 195, 255} : Color{206, 237, 223, 255}; // Coral hover / Soft mint
    DrawRectangleRec(customerButton, custBtnColor);
    DrawRectangleLinesEx(customerButton, 3, Color{85, 107, 95, 255}); // Dark forest green border

    const char* customerText = "CUSTOMER MODE";
    int customerTextSize = 30;
    int customerTextWidth = MeasureText(customerText, customerTextSize);
    DrawText(customerText,
             customerButton.x + (customerButton.width - customerTextWidth) / 2,
             customerButton.y + (customerButton.height - customerTextSize) / 2,
             customerTextSize,
             Color{85, 107, 95, 255}); // Dark forest green

    // Staff button
    Color staffBtnColor = staffButtonHovered ? Color{255, 200, 195, 255} : Color{230, 224, 237, 255}; // Coral hover / Soft lavender
    DrawRectangleRec(staffButton, staffBtnColor);
    DrawRectangleLinesEx(staffButton, 3, Color{85, 107, 95, 255}); // Dark forest green border
    
    const char* staffText = "STAFF MODE";
    int staffTextSize = 30;
    int staffTextWidth = MeasureText(staffText, staffTextSize);
    DrawText(staffText,
             staffButton.x + (staffButton.width - staffTextWidth) / 2,
             staffButton.y + (staffButton.height - staffTextSize) / 2,
             staffTextSize,
             Color{85, 107, 95, 255}); // Dark forest green
    
    // Draw hover hints
    if (customerButtonHovered) {
        const char* hint = "Shop for plants with a random budget";
        int hintSize = 18;
        int hintWidth = MeasureText(hint, hintSize);
        DrawText(hint,
                 (GetScreenWidth() - hintWidth) / 2,
                 customerButton.y + customerButton.height + 20,
                 hintSize,
                 Color{235, 186, 170, 255}); // Warm terracotta
    }

    if (staffButtonHovered) {
        const char* hint = "Manage plants and care for the greenhouse";
        int hintSize = 18;
        int hintWidth = MeasureText(hint, hintSize);
        DrawText(hint,
                 (GetScreenWidth() - hintWidth) / 2,
                 staffButton.y + staffButton.height + 20,
                 hintSize,
                 Color{235, 186, 170, 255}); // Warm terracotta
    }
}