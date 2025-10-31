#include "raylib.h"
#include "ScreenManager.h"
#include "StartScreen.h"
#include "SalesFloorScreen.h"
#include "CustomerGreenhouseScreen.h"
#include "StaffGreenhouseScreen.h"
#include "CartViewScreen.h"
#include "DecorationScreen.h"
#include "OrderCreationScreen.h"
// Add other screen includes as you create them

int main() {
    // Initialize window
    const int screenWidth = 1400;
    const int screenHeight = 900;
    
    InitWindow(screenWidth, screenHeight, "Plant Nursery Shop");
    SetTargetFPS(60);
    
    // Create screen manager
    ScreenManager manager;
    manager.Initialize();
    
    // Create screens
    StartScreen startScreen(&manager);
    SalesFloorScreen salesFloorScreen(&manager);
    CustomerGreenhouseScreen customerGreenhouseScreen(&manager);
    StaffGreenhouseScreen staffGreenhouseScreen(&manager);
    CartViewScreen cartViewScreen(&manager);
    DecorationScreen decorationScreen(&manager);
    OrderCreationScreen orderCreationScreen(&manager);

    // Track previous screen to detect transitions
    GameScreen lastScreen = manager.GetCurrentScreen();

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        manager.Update();
        
        // Detect screen transitions
        GameScreen currentScreen = manager.GetCurrentScreen();
        bool screenChanged = (currentScreen != lastScreen);

        // Draw based on current screen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen) {
            case GameScreen::START:
                startScreen.Update();
                startScreen.Draw();
                break;
                
            case GameScreen::SALES_FLOOR:
                salesFloorScreen.Update();
                salesFloorScreen.Draw();
                break;

            case GameScreen::CUSTOMER_GREENHOUSE:
                customerGreenhouseScreen.Update();
                customerGreenhouseScreen.Draw();
                break;

            case GameScreen::STAFF_GREENHOUSE:
                staffGreenhouseScreen.Update();
                staffGreenhouseScreen.Draw();
                break;

            case GameScreen::CART_VIEW:
                cartViewScreen.Update();
                cartViewScreen.Draw();
                break;
                
            case GameScreen::DECORATION:
                decorationScreen.Update();
                decorationScreen.Draw();
                break;

            case GameScreen::ORDER_CREATION:
                // Reset screen when first entering
                if (screenChanged) {
                    orderCreationScreen.Reset();
                }
                orderCreationScreen.Update();
                orderCreationScreen.Draw();
                break;

            // Add other screens as you implement them
            default:
                DrawText("Screen not implemented yet", 10, 10, 20, RED);
                break;
        }

        EndDrawing();

        // Update last screen for next frame
        lastScreen = currentScreen;
    }
    
    // Cleanup
    manager.Cleanup();
    CloseWindow();
    
    return 0;
}