#include "raylib.h"
#include "ScreenManager.h"
#include "StartScreen.h"
#include "SalesFloorScreen.h"
#include "CustomerGreenhouseScreen.h"
#include "StaffGreenhouseScreen.h"
#include "CartViewScreen.h"
#include "DecorationScreen.h"
#include "OrderCreationScreen.h"
#include "FinalOrderScreen.h"


int main() {
    const int screenWidth = 1400;
    const int screenHeight = 900;
    
    InitWindow(screenWidth, screenHeight, "Plant Nursery Shop");

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
    FinalOrderScreen finalOrderScreen(&manager);

    GameScreen lastScreen = manager.GetCurrentScreen();

    while (!WindowShouldClose()) {

        manager.Update();

        GameScreen currentScreen = manager.GetCurrentScreen();
        bool screenChanged = (currentScreen != lastScreen);

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
                if (screenChanged) {
                    orderCreationScreen.Reset();
                }
                orderCreationScreen.Update();
                orderCreationScreen.Draw();
                break;

            case GameScreen::CHECKOUT:
                if (screenChanged) {
                    finalOrderScreen.Reset();
                }
                finalOrderScreen.Update();
                finalOrderScreen.Draw();
                break;

            default:
                DrawText("Screen not implemented yet", 10, 10, 20, RED);
                break;
        }

        int fps = GetFPS();
        DrawText(TextFormat("FPS: %d", fps), screenWidth - 100, 10, 20, DARKGRAY);

        EndDrawing();

        lastScreen = currentScreen;
    }

    manager.Cleanup();
    CloseWindow();
    
    return 0;
}