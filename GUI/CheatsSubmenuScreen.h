#ifndef CHEATS_SUBMENU_SCREEN_H
#define CHEATS_SUBMENU_SCREEN_H

#include "raylib.h"
#include <vector>
#include <string>

class ScreenManager;

/**
 * @class CheatsSubmenuScreen
 * @brief Terminal-style submenu for actual gameplay cheats
 *
 * Provides options to modify game state:
 * - Infinite Budget
 * - Free Plants
 * - Stock Greenhouse with all plants
 */
class CheatsSubmenuScreen {
private:
    ScreenManager* manager;

    // Menu options
    std::vector<std::string> menuOptions;
    int selectedOption;

    // Terminal aesthetic (matching CheatMenuScreen)
    const Color terminalGreen = {0, 255, 0, 255};
    const Color terminalBlack = {0, 0, 0, 255};
    const Color selectedColor = {0, 200, 0, 255};
    const Color activatedColor = {255, 215, 0, 255};  // Gold for activated cheats

    // Cursor blink animation
    float blinkTimer;
    bool showCursor;

    // Cheat states
    bool infiniteBudgetActive;
    bool freePlantsActive;
    bool greenhouseStockedActive;
    bool maxOutPlantsActive;
    bool fillCartActive;
    bool clearGreenhouseActive;
    bool clearSalesFloorActive;

    void InitializeMenu();
    void HandleInput();
    void DrawTerminalBackground();
    void DrawMenuOptions();
    void ExecuteSelectedOption();

    // Cheat implementations
    void ActivateInfiniteBudget();
    void ActivateFreePlants();
    void StockGreenhouse();
    void MaxOutAllPlantStates();
    void FillCartWithRandomPlants();
    void ClearGreenhouse();
    void ClearSalesFloor();

public:
    CheatsSubmenuScreen(ScreenManager* mgr);
    ~CheatsSubmenuScreen();

    void Update();
    void Draw();
    void ResetCheats();  // Called when entering the screen
};

#endif // CHEATS_SUBMENU_SCREEN_H
