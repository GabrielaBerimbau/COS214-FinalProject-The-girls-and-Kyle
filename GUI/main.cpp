#include "raylib.h"

// Screens / manager
#include "ScreenManager.h"
#include "StartScreen.h"
#include "SalesFloorScreen.h"
#include "CustomerGreenhouseScreen.h"
#include "StaffGreenhouseScreen.h"
#include "CartViewScreen.h"
#include "DecorationScreen.h"
#include "OrderCreationScreen.h"
#include "FinalOrderScreen.h"
#include "StaffSalesFloorScreen.h"   // <-- NEW

int main() {
    const int screenWidth  = 1400;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Plant Nursery Shop");
    SetTargetFPS(120);

    ScreenManager manager;
    manager.Initialize();

    // Create screens (lifetime = whole program)
    StartScreen              startScreen(&manager);
    SalesFloorScreen         salesFloorScreen(&manager);
    CustomerGreenhouseScreen customerGreenhouseScreen(&manager);
    StaffGreenhouseScreen    staffGreenhouseScreen(&manager);
    StaffSalesFloorScreen    staffSalesFloorScreen(&manager);   // <-- NEW
    CartViewScreen           cartViewScreen(&manager);
    DecorationScreen         decorationScreen(&manager);
    OrderCreationScreen      orderCreationScreen(&manager);
    FinalOrderScreen         finalOrderScreen(&manager);

    GameScreen lastScreen = manager.GetCurrentScreen();

    while (!WindowShouldClose()) {
        // Global per-frame updates
        manager.Update();

        GameScreen currentScreen = manager.GetCurrentScreen();
        bool screenChanged = (currentScreen != lastScreen);

        BeginDrawing();
        ClearBackground(RAYWHITE); // individual screens draw their own backgrounds

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

            case GameScreen::STAFF_SALES_FLOOR:  // <-- NEW CASE
                staffSalesFloorScreen.Update();
                staffSalesFloorScreen.Draw();
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

        // HUD: FPS (top-right)
        DrawText(TextFormat("FPS: %d", GetFPS()), screenWidth - 100, 10, 20, Color{60, 60, 60, 255});
        EndDrawing();

        lastScreen = currentScreen;
    }

    manager.Cleanup();
    CloseWindow();
    return 0;
}
