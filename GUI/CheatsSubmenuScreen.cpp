#include "CheatsSubmenuScreen.h"
#include "ScreenManager.h"
#include "../include/Customer.h"
#include "../include/Greenhouse.h"
#include "../include/SalesFloor.h"
#include "../include/Plant.h"
#include "../include/RoseFactory.h"
#include "../include/DaisyFactory.h"
#include "../include/CactusFactory.h"
#include "../include/AloeFactory.h"
#include "../include/PotatoFactory.h"
#include "../include/StrelitziaFactory.h"
#include "../include/RadishFactory.h"
#include "../include/MonsteraFactory.h"
#include "../include/VenusFlyTrapFactory.h"
#include "../include/CarrotFactory.h"
#include "../include/MatureState.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

CheatsSubmenuScreen::CheatsSubmenuScreen(ScreenManager* mgr)
    : manager(mgr), selectedOption(0), blinkTimer(0.0f), showCursor(true),
      infiniteBudgetActive(false), freePlantsActive(false), greenhouseStockedActive(false),
      maxOutPlantsActive(false), fillCartActive(false), clearGreenhouseActive(false),
      clearSalesFloorActive(false) {
    InitializeMenu();
}

CheatsSubmenuScreen::~CheatsSubmenuScreen() {
}

void CheatsSubmenuScreen::InitializeMenu() {
    menuOptions.clear();
    menuOptions.push_back("INFINITE BUDGET");
    menuOptions.push_back("FREE PLANTS");
    menuOptions.push_back("STOCK GREENHOUSE");
    menuOptions.push_back("MAX OUT ALL PLANT STATES");
    menuOptions.push_back("FILL CART WITH RANDOM PLANTS");
    menuOptions.push_back("CLEAR GREENHOUSE");
    menuOptions.push_back("CLEAR SALES FLOOR");
    menuOptions.push_back("BACK");
    selectedOption = 0;
}

void CheatsSubmenuScreen::ResetCheats() {
    infiniteBudgetActive = false;
    freePlantsActive = false;
    greenhouseStockedActive = false;
    maxOutPlantsActive = false;
    fillCartActive = false;
    clearGreenhouseActive = false;
    clearSalesFloorActive = false;
}

void CheatsSubmenuScreen::HandleInput() {
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

    // Q to go back
    if (IsKeyPressed(KEY_Q)) {
        manager->SwitchScreen(GameScreen::CHEAT_MENU);
    }
}

void CheatsSubmenuScreen::ExecuteSelectedOption() {
    std::cout << "[CheatsSubmenu] Selected: " << menuOptions[selectedOption] << std::endl;

    if (menuOptions[selectedOption] == "INFINITE BUDGET") {
        ActivateInfiniteBudget();
    } else if (menuOptions[selectedOption] == "FREE PLANTS") {
        ActivateFreePlants();
    } else if (menuOptions[selectedOption] == "STOCK GREENHOUSE") {
        StockGreenhouse();
    } else if (menuOptions[selectedOption] == "MAX OUT ALL PLANT STATES") {
        MaxOutAllPlantStates();
    } else if (menuOptions[selectedOption] == "FILL CART WITH RANDOM PLANTS") {
        FillCartWithRandomPlants();
    } else if (menuOptions[selectedOption] == "CLEAR GREENHOUSE") {
        ClearGreenhouse();
    } else if (menuOptions[selectedOption] == "CLEAR SALES FLOOR") {
        ClearSalesFloor();
    } else if (menuOptions[selectedOption] == "BACK") {
        manager->SwitchScreen(GameScreen::CHEAT_MENU);
    }
}

void CheatsSubmenuScreen::ActivateInfiniteBudget() {
    Customer* customer = manager->GetCustomer();

    if (customer == nullptr) {
        std::cout << "[CHEAT] No customer exists! Start as customer first." << std::endl;
        return;
    }

    customer->setBudget(999999.99);
    infiniteBudgetActive = true;

    std::cout << "[CHEAT] Infinite Budget activated! Budget set to $999,999.99" << std::endl;
}

void CheatsSubmenuScreen::ActivateFreePlants() {
    SalesFloor* salesFloor = manager->GetSalesFloor();

    if (salesFloor == nullptr) {
        std::cout << "[CHEAT] Sales floor not available!" << std::endl;
        return;
    }

    // Get all plants on sales floor and set their price to 0
    std::vector<Plant*> plants = salesFloor->getDisplayPlants();
    int count = 0;

    for (Plant* plant : plants) {
        if (plant != nullptr) {
            plant->setPrice(0.0);
            count++;
        }
    }

    freePlantsActive = true;
    std::cout << "[CHEAT] Free Plants activated! Set " << count << " plants to $0.00" << std::endl;
}

void CheatsSubmenuScreen::StockGreenhouse() {
    Greenhouse* greenhouse = manager->GetGreenhouse();

    if (greenhouse == nullptr) {
        std::cout << "[CHEAT] Greenhouse not available!" << std::endl;
        return;
    }

    // Create all plant types using factories
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    AloeFactory aloeFactory;
    PotatoFactory potatoFactory;
    StrelitziaFactory strelitziaFactory;
    RadishFactory radishFactory;
    MonsteraFactory monsteraFactory;
    VenusFlyTrapFactory vftFactory;
    CarrotFactory carrotFactory;

    std::vector<PlantFactory*> factories = {
        &roseFactory, &daisyFactory, &cactusFactory, &aloeFactory, &potatoFactory,
        &strelitziaFactory, &radishFactory, &monsteraFactory, &vftFactory, &carrotFactory
    };

    int count = 0;
    int rows = greenhouse->getRows();
    int cols = greenhouse->getColumns();

    // Add 2 of each plant type to greenhouse - find empty positions
    for (PlantFactory* factory : factories) {
        for (int i = 0; i < 2; i++) {
            // Find an empty position
            bool added = false;
            for (int row = 0; row < rows && !added; row++) {
                for (int col = 0; col < cols && !added; col++) {
                    if (greenhouse->isPositionEmpty(row, col)) {
                        Plant* plant = factory->buildPlant(manager->GetCareScheduler());
                        if (plant != nullptr) {
                            if (greenhouse->addPlant(plant, row, col)) {
                                count++;
                                added = true;
                            }
                        }
                    }
                }
            }
        }
    }

    greenhouseStockedActive = true;
    std::cout << "[CHEAT] Stock Greenhouse activated! Added " << count << " plants to greenhouse" << std::endl;
}

void CheatsSubmenuScreen::MaxOutAllPlantStates() {
    Greenhouse* greenhouse = manager->GetGreenhouse();

    if (greenhouse == nullptr) {
        std::cout << "[CHEAT] Greenhouse not available!" << std::endl;
        return;
    }

    // Get all plants from greenhouse and set them to mature state
    std::vector<Plant*> plants = greenhouse->getAllPlants();
    int count = 0;

    for (Plant* plant : plants) {
        if (plant != nullptr) {
            // Set to mature state
            plant->setState(new MatureState());
            // Set optimal conditions for mature plants
            plant->setWaterLevel(80);
            plant->setSunlightExposure(8);
            plant->setNutrientLevel(80);
            plant->setReadyForSale(true);
            count++;
        }
    }

    maxOutPlantsActive = true;
    std::cout << "[CHEAT] Max Out All Plant States activated! Set " << count << " plants to Mature state" << std::endl;
}

void CheatsSubmenuScreen::FillCartWithRandomPlants() {
    Customer* customer = manager->GetCustomer();
    SalesFloor* salesFloor = manager->GetSalesFloor();

    if (customer == nullptr) {
        std::cout << "[CHEAT] No customer exists! Start as customer first." << std::endl;
        return;
    }

    if (salesFloor == nullptr) {
        std::cout << "[CHEAT] Sales floor not available!" << std::endl;
        return;
    }

    // Get all plants from sales floor
    std::vector<Plant*> plants = salesFloor->getDisplayPlants();

    if (plants.empty()) {
        std::cout << "[CHEAT] No plants on sales floor to add!" << std::endl;
        return;
    }

    // Add 5 random plants (or all available if less than 5)
    int plantsToAdd = (plants.size() < 5) ? plants.size() : 5;
    int count = 0;

    // Seed random number generator
    std::srand(std::time(nullptr));

    for (int i = 0; i < plantsToAdd; i++) {
        int randomIndex = std::rand() % plants.size();
        Plant* plant = plants[randomIndex];

        if (plant != nullptr) {
            // Add by name
            if (customer->addPlantFromSalesFloor(plant->getName())) {
                count++;
            }
            // Remove from vector to avoid duplicates
            plants.erase(plants.begin() + randomIndex);
        }
    }

    fillCartActive = true;
    std::cout << "[CHEAT] Fill Cart activated! Added " << count << " random plants to cart" << std::endl;
}

void CheatsSubmenuScreen::ClearGreenhouse() {
    Greenhouse* greenhouse = manager->GetGreenhouse();

    if (greenhouse == nullptr) {
        std::cout << "[CHEAT] Greenhouse not available!" << std::endl;
        return;
    }

    int rows = greenhouse->getRows();
    int cols = greenhouse->getColumns();
    int count = 0;

    // Remove all plants from greenhouse grid
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Plant* plant = greenhouse->removePlantAt(row, col);
            if (plant != nullptr) {
                delete plant;
                count++;
            }
        }
    }

    clearGreenhouseActive = true;
    std::cout << "[CHEAT] Clear Greenhouse activated! Removed " << count << " plants from greenhouse" << std::endl;
}

void CheatsSubmenuScreen::ClearSalesFloor() {
    SalesFloor* salesFloor = manager->GetSalesFloor();

    if (salesFloor == nullptr) {
        std::cout << "[CHEAT] Sales floor not available!" << std::endl;
        return;
    }

    int rows = salesFloor->getRows();
    int cols = salesFloor->getColumns();
    int count = 0;

    // Remove all plants from sales floor grid
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Plant* plant = salesFloor->removePlantAt(row, col);
            if (plant != nullptr) {
                delete plant;
                count++;
            }
        }
    }

    clearSalesFloorActive = true;
    std::cout << "[CHEAT] Clear Sales Floor activated! Removed " << count << " plants from sales floor" << std::endl;
}

void CheatsSubmenuScreen::Update() {
    HandleInput();

    // Cursor blink animation
    blinkTimer += GetFrameTime();
    if (blinkTimer >= 0.5f) {
        showCursor = !showCursor;
        blinkTimer = 0.0f;
    }
}

void CheatsSubmenuScreen::DrawTerminalBackground() {
    ClearBackground(terminalBlack);

    // Draw scanlines for CRT effect
    for (int y = 0; y < GetScreenHeight(); y += 4) {
        DrawLine(0, y, GetScreenWidth(), y, Color{0, 20, 0, 50});
    }
}

void CheatsSubmenuScreen::DrawMenuOptions() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Draw title
    const char* title = "=== CHEATS ===";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, screenWidth / 2 - titleWidth / 2, 100, 40, terminalGreen);

    // Draw subtitle
    const char* subtitle = "WARNING: These modify your game state!";
    int subtitleWidth = MeasureText(subtitle, 18);
    DrawText(subtitle, screenWidth / 2 - subtitleWidth / 2, 150, 18, Color{255, 100, 100, 255});

    // Draw menu options
    int startY = screenHeight / 2 - (menuOptions.size() * 30);
    for (size_t i = 0; i < menuOptions.size(); i++) {
        Color color = (i == (size_t)selectedOption) ? selectedColor : terminalGreen;

        // Check if cheat is active and change color to gold
        if (menuOptions[i] == "INFINITE BUDGET" && infiniteBudgetActive) {
            color = activatedColor;
        } else if (menuOptions[i] == "FREE PLANTS" && freePlantsActive) {
            color = activatedColor;
        } else if (menuOptions[i] == "STOCK GREENHOUSE" && greenhouseStockedActive) {
            color = activatedColor;
        } else if (menuOptions[i] == "MAX OUT ALL PLANT STATES" && maxOutPlantsActive) {
            color = activatedColor;
        } else if (menuOptions[i] == "FILL CART WITH RANDOM PLANTS" && fillCartActive) {
            color = activatedColor;
        } else if (menuOptions[i] == "CLEAR GREENHOUSE" && clearGreenhouseActive) {
            color = activatedColor;
        } else if (menuOptions[i] == "CLEAR SALES FLOOR" && clearSalesFloorActive) {
            color = activatedColor;
        }

        std::string prefix = (i == (size_t)selectedOption) ? "> " : "  ";
        std::string suffix = "";

        // Add status indicators
        if (menuOptions[i] == "INFINITE BUDGET" && infiniteBudgetActive) {
            suffix = " [ACTIVE]";
        } else if (menuOptions[i] == "FREE PLANTS" && freePlantsActive) {
            suffix = " [ACTIVE]";
        } else if (menuOptions[i] == "STOCK GREENHOUSE" && greenhouseStockedActive) {
            suffix = " [ACTIVE]";
        } else if (menuOptions[i] == "MAX OUT ALL PLANT STATES" && maxOutPlantsActive) {
            suffix = " [ACTIVE]";
        } else if (menuOptions[i] == "FILL CART WITH RANDOM PLANTS" && fillCartActive) {
            suffix = " [ACTIVE]";
        } else if (menuOptions[i] == "CLEAR GREENHOUSE" && clearGreenhouseActive) {
            suffix = " [ACTIVE]";
        } else if (menuOptions[i] == "CLEAR SALES FLOOR" && clearSalesFloorActive) {
            suffix = " [ACTIVE]";
        }

        std::string text = prefix + menuOptions[i] + suffix;

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
    const char* instructions = "ARROW KEYS - Navigate | ENTER - Activate | Q - Back";
    int instrWidth = MeasureText(instructions, 15);
    DrawText(instructions, screenWidth / 2 - instrWidth / 2, screenHeight - 50, 15, terminalGreen);

    // Draw additional info
    const char* info = "Note: Start as a customer before using budget/plant cheats";
    int infoWidth = MeasureText(info, 14);
    DrawText(info, screenWidth / 2 - infoWidth / 2, screenHeight - 80, 14, Color{150, 150, 150, 255});
}

void CheatsSubmenuScreen::Draw() {
    DrawTerminalBackground();
    DrawMenuOptions();
}
