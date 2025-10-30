#include "raylib.h"
#include "ScreenManager.h"
#include "StartScreen.h"
#include "SalesFloorScreen.h"
#include "CustomerGreenhouseScreen.h"
#include "StaffGreenhouseScreen.h"
#include "CartViewScreen.h"
#include "DecorationScreen.h"
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
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        manager.Update();
        
        // Draw based on current screen
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        switch (manager.GetCurrentScreen()) {
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
                
            // Add other screens as you implement them
            default:
                DrawText("Screen not implemented yet", 10, 10, 20, RED);
                break;
        }
        
        EndDrawing();
    }
    
    // Cleanup
    manager.Cleanup();
    CloseWindow();
    
    return 0;
}