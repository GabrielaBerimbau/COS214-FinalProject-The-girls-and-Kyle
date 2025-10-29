#include "StaffGreenhouseScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Backend includes
#include "../include/Greenhouse.h"
#include "../include/SalesFloor.h"
#include "../include/Plant.h"
#include "../include/CareStrategy.h"
#include "../include/CareScheduler.h"
#include "../include/NurseryCoordinator.h"

StaffGreenhouseScreen::StaffGreenhouseScreen(ScreenManager* mgr)
    : manager(mgr),
      selectedRow(-1),
      selectedCol(-1),
      selectedPlant(nullptr),
      waterHovered(false),
      fertilizeHovered(false),
      sunlightHovered(false),
      fullCareHovered(false),
      transferHovered(false),
      schedulerHovered(false),
      advanceDayHovered(false),
      backHovered(false),
      scheduledTasksCount(0),
      schedulerOverlayActive(false),
      schedulerTasksExecuted(false),
      runAllHovered(false),
      runOneHovered(false),
      closeOverlayHovered(false) {
    
    gridRows = 6;
    gridCols = 8;
    
    InitializeLayout();
    InitializeButtons();
}

StaffGreenhouseScreen::~StaffGreenhouseScreen() {
}

void StaffGreenhouseScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    leftPanelWidth = screenWidth / 5;
    middlePanelWidth = screenWidth * 3 / 5;
    rightPanelWidth = screenWidth / 5;
    
    int availableWidth = middlePanelWidth - 20;
    int availableHeight = screenHeight - 80;
    
    int cellSizeByWidth = availableWidth / gridCols;
    int cellSizeByHeight = availableHeight / gridRows;
    
    cellSize = (cellSizeByWidth < cellSizeByHeight) ? cellSizeByWidth : cellSizeByHeight;
    
    int gridTotalWidth = cellSize * gridCols;
    int gridTotalHeight = cellSize * gridRows;
    
    gridStartX = leftPanelWidth + (middlePanelWidth - gridTotalWidth) / 2;
    gridStartY = 60 + (screenHeight - 60 - gridTotalHeight) / 2;
}

void StaffGreenhouseScreen::InitializeButtons() {
    int buttonWidth = rightPanelWidth - 40;
    int buttonHeight = 40;
    int buttonSpacing = 10;
    int buttonX = screenWidth - rightPanelWidth + 20;
    int startY = 60;
    
    waterButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    fertilizeButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing)),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    adjustSunlightButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 2),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    performFullCareButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 3 + 10),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight + 10)
    };
    
    transferToSalesFloorButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 4 + 30),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight + 10)
    };
    
    advanceDayButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(screenHeight - 200),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight + 10)
    };
    
    runSchedulerButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(screenHeight - 130),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    backButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(screenHeight - 70),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
}

void StaffGreenhouseScreen::Update() {
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr) {
        scheduledTasksCount = scheduler->empty() ? 0 : 1;
    }
    
    if (schedulerOverlayActive) {
        UpdateSchedulerOverlay();
    } else {
        UpdateGrid();
        UpdateButtons();
    }
}

void StaffGreenhouseScreen::UpdateGrid() {
    Vector2 mousePos = GetMousePosition();
    
    if (mousePos.x >= gridStartX && mousePos.x < gridStartX + (cellSize * gridCols) &&
        mousePos.y >= gridStartY && mousePos.y < gridStartY + (cellSize * gridRows)) {
        
        int col = static_cast<int>((mousePos.x - gridStartX) / cellSize);
        int row = static_cast<int>((mousePos.y - gridStartY) / cellSize);
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            HandlePlantSelection(row, col);
        }
    }
}

void StaffGreenhouseScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();
    
    waterHovered = CheckCollisionPointRec(mousePos, waterButton);
    fertilizeHovered = CheckCollisionPointRec(mousePos, fertilizeButton);
    sunlightHovered = CheckCollisionPointRec(mousePos, adjustSunlightButton);
    fullCareHovered = CheckCollisionPointRec(mousePos, performFullCareButton);
    transferHovered = CheckCollisionPointRec(mousePos, transferToSalesFloorButton);
    advanceDayHovered = CheckCollisionPointRec(mousePos, advanceDayButton);
    schedulerHovered = CheckCollisionPointRec(mousePos, runSchedulerButton);
    backHovered = CheckCollisionPointRec(mousePos, backButton);
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (waterHovered && selectedPlant != nullptr) {
            HandleWaterPlant();
        } else if (fertilizeHovered && selectedPlant != nullptr) {
            HandleFertilizePlant();
        } else if (sunlightHovered && selectedPlant != nullptr) {
            HandleAdjustSunlight();
        } else if (fullCareHovered && selectedPlant != nullptr) {
            HandleFullCare();
        } else if (transferHovered && selectedPlant != nullptr) {
            HandleTransferToSalesFloor();
        } else if (advanceDayHovered) {
            HandleAdvanceDay();
        } else if (schedulerHovered) {
            HandleRunScheduler();
        } else if (backHovered) {
            manager->SwitchScreen(GameScreen::START);
        }
    }
}

void StaffGreenhouseScreen::UpdateSchedulerOverlay() {
    Vector2 mousePos = GetMousePosition();
    
    runAllHovered = CheckCollisionPointRec(mousePos, runAllButton);
    runOneHovered = CheckCollisionPointRec(mousePos, runOneButton);
    closeOverlayHovered = CheckCollisionPointRec(mousePos, closeOverlayButton);
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (runAllHovered) {
            HandleRunAllScheduled();
        } else if (runOneHovered) {
            HandleRunOneScheduled();
        } else if (closeOverlayHovered) {
            // Close and advance day if we executed tasks
            if (schedulerTasksExecuted) {
                std::cout << "[StaffGreenhouseScreen] Closing overlay and advancing day..." << std::endl;
                manager->PerformDailyUpdate();
                std::cout << "[StaffGreenhouseScreen] Day advanced to: " << manager->GetDaysCounter() << std::endl;
            }
            schedulerOverlayActive = false;
            schedulerTasksExecuted = false;
        }
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        // Close and advance day if we executed tasks
        if (schedulerTasksExecuted) {
            std::cout << "[StaffGreenhouseScreen] Closing overlay (ESC) and advancing day..." << std::endl;
            manager->PerformDailyUpdate();
            std::cout << "[StaffGreenhouseScreen] Day advanced to: " << manager->GetDaysCounter() << std::endl;
        }
        schedulerOverlayActive = false;
        schedulerTasksExecuted = false;
    }
}

void StaffGreenhouseScreen::HandlePlantSelection(int row, int col) {
    Greenhouse* greenhouse = manager->GetGreenhouse();
    if (greenhouse == nullptr) return;
    
    Plant* plant = greenhouse->getPlantAt(row, col);
    
    if (plant != nullptr) {
        selectedRow = row;
        selectedCol = col;
        selectedPlant = plant;
        std::cout << "[StaffGreenhouseScreen] Selected plant: " << plant->getName() 
                  << " at (" << row << "," << col << ")" << std::endl;
    } else {
        selectedRow = -1;
        selectedCol = -1;
        selectedPlant = nullptr;
    }
}

void StaffGreenhouseScreen::HandleWaterPlant() {
    if (selectedPlant == nullptr) return;
    
    CareStrategy* strategy = selectedPlant->getStrategy();
    if (strategy != nullptr) {
        strategy->water(selectedPlant);
        selectedPlant->updateHealth();
        std::cout << "[StaffGreenhouseScreen] Watered plant " << selectedPlant->getID() 
                  << " - Water: " << selectedPlant->getWaterLevel() 
                  << "%, Health: " << selectedPlant->getHealthLevel() << "%" << std::endl;
    }
}

void StaffGreenhouseScreen::HandleFertilizePlant() {
    if (selectedPlant == nullptr) return;
    
    CareStrategy* strategy = selectedPlant->getStrategy();
    if (strategy != nullptr) {
        strategy->fertilize(selectedPlant);
        selectedPlant->updateHealth();
        std::cout << "[StaffGreenhouseScreen] Fertilized plant " << selectedPlant->getID() 
                  << " - Nutrients: " << selectedPlant->getNutrientLevel() 
                  << "%, Health: " << selectedPlant->getHealthLevel() << "%" << std::endl;
    }
}

void StaffGreenhouseScreen::HandleAdjustSunlight() {
    if (selectedPlant == nullptr) return;
    
    CareStrategy* strategy = selectedPlant->getStrategy();
    if (strategy != nullptr) {
        strategy->adjustSunlight(selectedPlant);
        selectedPlant->updateHealth();
        std::cout << "[StaffGreenhouseScreen] Adjusted sunlight for plant " << selectedPlant->getID() 
                  << " - Sunlight: " << selectedPlant->getSunlightExposure() 
                  << "%, Health: " << selectedPlant->getHealthLevel() << "%" << std::endl;
    }
}

void StaffGreenhouseScreen::HandleFullCare() {
    if (selectedPlant == nullptr) return;
    
    selectedPlant->performCare();
    selectedPlant->updateHealth();
    std::cout << "[StaffGreenhouseScreen] Performed full care on plant " << selectedPlant->getID() 
              << " - Health: " << selectedPlant->getHealthLevel() << "%" << std::endl;
}

void StaffGreenhouseScreen::HandleTransferToSalesFloor() {
    if (selectedPlant == nullptr) return;
    
    if (!selectedPlant->isReadyForSale()) {
        std::cout << "[StaffGreenhouseScreen] Plant " << selectedPlant->getID() 
                  << " is not ready for sale yet! (State: " 
                  << selectedPlant->getState()->getStateName() << ")" << std::endl;
        return;
    }
    
    Greenhouse* greenhouse = manager->GetGreenhouse();
    SalesFloor* salesFloor = manager->GetSalesFloor();
    
    if (greenhouse == nullptr || salesFloor == nullptr) {
        std::cout << "[StaffGreenhouseScreen] Error: Greenhouse or SalesFloor not found" << std::endl;
        return;
    }
    
    bool transferred = false;
    for (int row = 0; row < salesFloor->getRows() && !transferred; row++) {
        for (int col = 0; col < salesFloor->getColumns() && !transferred; col++) {
            if (salesFloor->isPositionEmpty(row, col)) {
                greenhouse->removePlant(selectedPlant);
                salesFloor->addPlantToDisplay(selectedPlant, row, col);
                
                std::cout << "[StaffGreenhouseScreen] Transferred " << selectedPlant->getName() 
                          << " (ID: " << selectedPlant->getID() 
                          << ") to sales floor at (" << row << "," << col << ")" << std::endl;
                
                selectedPlant = nullptr;
                selectedRow = -1;
                selectedCol = -1;
                
                transferred = true;
            }
        }
    }
    
    if (!transferred) {
        std::cout << "[StaffGreenhouseScreen] Sales floor is full! Cannot transfer plant." << std::endl;
    }
}

void StaffGreenhouseScreen::HandleAdvanceDay() {
    std::cout << "[StaffGreenhouseScreen] Advancing day manually..." << std::endl;
    manager->PerformDailyUpdate();
    std::cout << "[StaffGreenhouseScreen] Day advanced to: " << manager->GetDaysCounter() << std::endl;
}

void StaffGreenhouseScreen::HandleRunScheduler() {
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        schedulerOverlayActive = true;
        schedulerTasksExecuted = false;
        std::cout << "[StaffGreenhouseScreen] Opening scheduler overlay..." << std::endl;
    } else {
        std::cout << "[StaffGreenhouseScreen] No tasks in scheduler" << std::endl;
    }
}

void StaffGreenhouseScreen::HandleRunAllScheduled() {
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        std::cout << "[StaffGreenhouseScreen] Running all scheduled tasks..." << std::endl;
        scheduler->runAll();
        schedulerTasksExecuted = true;
        std::cout << "[StaffGreenhouseScreen] All tasks completed. Close overlay to advance day." << std::endl;
        schedulerOverlayActive = false;
        
        // Advance day immediately after running all
        manager->PerformDailyUpdate();
        std::cout << "[StaffGreenhouseScreen] Day advanced to: " << manager->GetDaysCounter() << std::endl;
        schedulerTasksExecuted = false;
    }
}

void StaffGreenhouseScreen::HandleRunOneScheduled() {
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        std::cout << "[StaffGreenhouseScreen] Running one scheduled task..." << std::endl;
        scheduler->runNext();
        schedulerTasksExecuted = true;
        
        if (scheduler->empty()) {
            std::cout << "[StaffGreenhouseScreen] All tasks completed" << std::endl;
            schedulerOverlayActive = false;
            
            // Advance day after completing all tasks
            manager->PerformDailyUpdate();
            std::cout << "[StaffGreenhouseScreen] Day advanced to: " << manager->GetDaysCounter() << std::endl;
            schedulerTasksExecuted = false;
        }
    }
}

void StaffGreenhouseScreen::Draw() {
    ClearBackground(GRAY);
    
    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();
    
    if (schedulerOverlayActive) {
        DrawSchedulerOverlay();
    }
}

void StaffGreenhouseScreen::DrawLeftPanel() {
    DrawRectangle(0, 0, leftPanelWidth, screenHeight, Color{40, 40, 50, 255});
    DrawLine(leftPanelWidth, 0, leftPanelWidth, screenHeight, BLACK);
    
    int yPos = 20;
    
    DrawText("STAFF MODE", 20, yPos, 20, YELLOW);
    yPos += 35;
    
    DrawText("Greenhouse Manager", 20, yPos, 16, LIGHTGRAY);
    yPos += 30;
    
    DrawDayCounter();
    yPos += 60;
    
    DrawLine(20, yPos, leftPanelWidth - 20, yPos, WHITE);
    yPos += 15;
    
    if (selectedPlant != nullptr) {
        DrawText("SELECTED PLANT", 20, yPos, 18, YELLOW);
        yPos += 30;
        
        std::string plantNameText = selectedPlant->getName();
        DrawText(plantNameText.c_str(), 20, yPos, 16, WHITE);
        yPos += 25;
        
        std::string idText = "ID: " + selectedPlant->getID();
        DrawText(idText.c_str(), 20, yPos, 13, LIGHTGRAY);
        yPos += 25;
        
        std::string stateText = "State: " + selectedPlant->getState()->getStateName();
        DrawText(stateText.c_str(), 20, yPos, 13, LIGHTGRAY);
        yPos += 25;
        
        std::ostringstream ageStream;
        ageStream << "Age: " << selectedPlant->getAge() << " days";
        DrawText(ageStream.str().c_str(), 20, yPos, 13, LIGHTGRAY);
        yPos += 30;
        
        DrawLine(20, yPos, leftPanelWidth - 20, yPos, DARKGRAY);
        yPos += 15;
        
        DrawText("PLANT STATUS:", 20, yPos, 14, SKYBLUE);
        yPos += 25;
        
        std::ostringstream healthStream;
        healthStream << "Health: " << selectedPlant->getHealthLevel() << "%";
        Color healthColor = selectedPlant->getHealthLevel() > 70 ? GREEN : 
                           selectedPlant->getHealthLevel() > 40 ? YELLOW : RED;
        DrawText(healthStream.str().c_str(), 20, yPos, 14, healthColor);
        yPos += 22;
        
        std::ostringstream waterStream;
        waterStream << "Water: " << selectedPlant->getWaterLevel() << "%";
        Color waterColor = selectedPlant->getWaterLevel() > 50 ? SKYBLUE : 
                          selectedPlant->getWaterLevel() > 25 ? ORANGE : RED;
        DrawText(waterStream.str().c_str(), 20, yPos, 14, waterColor);
        yPos += 22;
        
        std::ostringstream nutrientStream;
        nutrientStream << "Nutrients: " << selectedPlant->getNutrientLevel() << "%";
        Color nutrientColor = selectedPlant->getNutrientLevel() > 50 ? Color{150, 255, 150, 255} : 
                             selectedPlant->getNutrientLevel() > 25 ? ORANGE : RED;
        DrawText(nutrientStream.str().c_str(), 20, yPos, 14, nutrientColor);
        yPos += 22;
        
        std::ostringstream sunStream;
        sunStream << "Sunlight: " << selectedPlant->getSunlightExposure() << "%";
        DrawText(sunStream.str().c_str(), 20, yPos, 14, GOLD);
        yPos += 30;
        
    } else {
        DrawText("INSTRUCTIONS:", 20, yPos, 16, YELLOW);
        yPos += 30;
        
        DrawText("1. Click a plant to select", 20, yPos, 13, WHITE);
        yPos += 22;
        DrawText("2. Use care buttons", 20, yPos, 13, WHITE);
        yPos += 22;
        DrawText("3. Monitor health status", 20, yPos, 13, WHITE);
        yPos += 22;
        DrawText("4. Transfer when ready", 20, yPos, 13, WHITE);
        yPos += 18;
        DrawText("   for sale", 20, yPos, 13, WHITE);
    }
}

void StaffGreenhouseScreen::DrawMiddlePanel() {
    DrawRectangle(leftPanelWidth, 0, middlePanelWidth, screenHeight, Color{60, 80, 70, 255});
    DrawLine(leftPanelWidth + middlePanelWidth, 0, leftPanelWidth + middlePanelWidth, screenHeight, BLACK);
    
    const char* header = "STAFF GREENHOUSE - PLANT CARE";
    int headerSize = 22;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header, 
             leftPanelWidth + (middlePanelWidth - headerWidth) / 2, 
             20, 
             headerSize, 
             WHITE);
    
    DrawGrid();
}

void StaffGreenhouseScreen::DrawRightPanel() {
    DrawRectangle(leftPanelWidth + middlePanelWidth, 0, rightPanelWidth, screenHeight, Color{30, 50, 40, 255});
    
    const char* header = "CARE ACTIONS";
    int headerSize = 18;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header, 
             screenWidth - rightPanelWidth + (rightPanelWidth - headerWidth) / 2, 
             20, 
             headerSize, 
             WHITE);
    
    DrawButtons();
    DrawSchedulerInfo();
}

void StaffGreenhouseScreen::DrawGrid() {
    Greenhouse* greenhouse = manager->GetGreenhouse();
    if (greenhouse == nullptr) return;
    
    for (int row = 0; row < gridRows; row++) {
        for (int col = 0; col < gridCols; col++) {
            int x = gridStartX + col * cellSize;
            int y = gridStartY + row * cellSize;
            
            Color cellColor = Color{50, 70, 60, 255};
            if (row == selectedRow && col == selectedCol) {
                cellColor = GOLD;
            }
            
            DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, cellColor);
            DrawRectangleLines(x + 1, y + 1, cellSize - 2, cellSize - 2, Color{40, 60, 50, 255});
            
            Plant* plant = greenhouse->getPlantAt(row, col);
            if (plant != nullptr) {
                DrawPlantInCell(plant, row, col);
            }
        }
    }
}

void StaffGreenhouseScreen::DrawPlantInCell(Plant* plant, int row, int col) {
    int x = gridStartX + col * cellSize;
    int y = gridStartY + row * cellSize;
    
    Texture2D plantTexture = manager->GetPlantTexture(plant->getName());
    
    if (plantTexture.id != 0) {
        int maxSize = cellSize - 20;
        float scale = static_cast<float>(maxSize) / plantTexture.height;
        
        int scaledWidth = static_cast<int>(plantTexture.width * scale);
        int scaledHeight = static_cast<int>(plantTexture.height * scale);
        
        int texX = x + (cellSize - scaledWidth) / 2;
        int texY = y + (cellSize - scaledHeight) / 2 - 5;
        
        DrawTextureEx(plantTexture, Vector2{static_cast<float>(texX), static_cast<float>(texY)}, 0.0f, scale, WHITE);
    } else {
        const char* name = plant->getName().c_str();
        int nameWidth = MeasureText(name, 10);
        DrawText(name, x + (cellSize - nameWidth) / 2, y + cellSize / 2 - 10, 10, DARKGREEN);
    }
    
    int healthBarWidth = cellSize - 10;
    int healthBarHeight = 4;
    int healthBarX = x + 5;
    int healthBarY = y + cellSize - 8;
    
    int health = plant->getHealthLevel();
    Color healthColor = health > 70 ? GREEN : health > 40 ? YELLOW : RED;
    
    DrawRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight, DARKGRAY);
    DrawRectangle(healthBarX, healthBarY, (healthBarWidth * health) / 100, healthBarHeight, healthColor);
}

void StaffGreenhouseScreen::DrawButtons() {
    bool hasSelection = selectedPlant != nullptr;
    
    Color waterColor = hasSelection ? 
                       (waterHovered ? Color{0, 150, 255, 255} : SKYBLUE) : GRAY;
    DrawRectangleRec(waterButton, waterColor);
    DrawRectangleLinesEx(waterButton, 2, BLACK);
    const char* waterText = "Water";
    int waterTextWidth = MeasureText(waterText, 16);
    DrawText(waterText,
             waterButton.x + (waterButton.width - waterTextWidth) / 2,
             waterButton.y + (waterButton.height - 16) / 2,
             16,
             hasSelection ? WHITE : DARKGRAY);
    
    Color fertColor = hasSelection ? 
                      (fertilizeHovered ? Color{100, 200, 100, 255} : Color{150, 255, 150, 255}) : GRAY;
    DrawRectangleRec(fertilizeButton, fertColor);
    DrawRectangleLinesEx(fertilizeButton, 2, BLACK);
    const char* fertText = "Fertilize";
    int fertTextWidth = MeasureText(fertText, 16);
    DrawText(fertText,
             fertilizeButton.x + (fertilizeButton.width - fertTextWidth) / 2,
             fertilizeButton.y + (fertilizeButton.height - 16) / 2,
             16,
             hasSelection ? BLACK : DARKGRAY);
    
    Color sunColor = hasSelection ? 
                     (sunlightHovered ? Color{255, 180, 0, 255} : GOLD) : GRAY;
    DrawRectangleRec(adjustSunlightButton, sunColor);
    DrawRectangleLinesEx(adjustSunlightButton, 2, BLACK);
    const char* sunText = "Sunlight";
    int sunTextWidth = MeasureText(sunText, 16);
    DrawText(sunText,
             adjustSunlightButton.x + (adjustSunlightButton.width - sunTextWidth) / 2,
             adjustSunlightButton.y + (adjustSunlightButton.height - 16) / 2,
             16,
             hasSelection ? BLACK : DARKGRAY);
    
    Color fullCareColor = hasSelection ? 
                          (fullCareHovered ? Color{100, 50, 150, 255} : Color{150, 100, 200, 255}) : GRAY;
    DrawRectangleRec(performFullCareButton, fullCareColor);
    DrawRectangleLinesEx(performFullCareButton, 3, BLACK);
    const char* fullCareText = "FULL CARE";
    int fullCareTextWidth = MeasureText(fullCareText, 18);
    DrawText(fullCareText,
             performFullCareButton.x + (performFullCareButton.width - fullCareTextWidth) / 2,
             performFullCareButton.y + (performFullCareButton.height - 18) / 2,
             18,
             WHITE);
    
    bool canTransfer = hasSelection && selectedPlant->isReadyForSale();
    Color transferColor = canTransfer ? 
                          (transferHovered ? Color{0, 120, 200, 255} : Color{50, 150, 220, 255}) : GRAY;
    DrawRectangleRec(transferToSalesFloorButton, transferColor);
    DrawRectangleLinesEx(transferToSalesFloorButton, 3, BLACK);
    const char* transferText = "To Sales Floor";
    int transferTextWidth = MeasureText(transferText, 16);
    DrawText(transferText,
             transferToSalesFloorButton.x + (transferToSalesFloorButton.width - transferTextWidth) / 2,
             transferToSalesFloorButton.y + (transferToSalesFloorButton.height - 16) / 2,
             16,
             canTransfer ? WHITE : DARKGRAY);
    
    Color advanceDayColor = advanceDayHovered ? Color{255, 200, 0, 255} : Color{255, 150, 0, 255};
    DrawRectangleRec(advanceDayButton, advanceDayColor);
    DrawRectangleLinesEx(advanceDayButton, 3, BLACK);
    const char* advanceDayText = "Advance Day";
    int advanceDayTextWidth = MeasureText(advanceDayText, 16);
    DrawText(advanceDayText,
             advanceDayButton.x + (advanceDayButton.width - advanceDayTextWidth) / 2,
             advanceDayButton.y + (advanceDayButton.height - 16) / 2,
             16,
             BLACK);
    
    Color schedulerColor = schedulerHovered ? Color{200, 150, 50, 255} : Color{150, 100, 50, 255};
    DrawRectangleRec(runSchedulerButton, schedulerColor);
    DrawRectangleLinesEx(runSchedulerButton, 2, BLACK);
    const char* schedulerText = "Run Scheduler";
    int schedulerTextWidth = MeasureText(schedulerText, 15);
    DrawText(schedulerText,
             runSchedulerButton.x + (runSchedulerButton.width - schedulerTextWidth) / 2,
             runSchedulerButton.y + (runSchedulerButton.height - 15) / 2,
             15,
             WHITE);
    
    Color backColor = backHovered ? DARKGRAY : Color{100, 100, 100, 255};
    DrawRectangleRec(backButton, backColor);
    DrawRectangleLinesEx(backButton, 2, BLACK);
    const char* backText = "Back";
    int backTextWidth = MeasureText(backText, 16);
    DrawText(backText,
             backButton.x + (backButton.width - backTextWidth) / 2,
             backButton.y + (backButton.height - 16) / 2,
             16,
             WHITE);
}

void StaffGreenhouseScreen::DrawSchedulerInfo() {
    int yPos = screenHeight - 250;
    
    DrawLine(screenWidth - rightPanelWidth + 20, yPos, screenWidth - 20, yPos, DARKGRAY);
    yPos += 10;
    
    DrawText("SCHEDULER:", screenWidth - rightPanelWidth + 20, yPos, 14, YELLOW);
    yPos += 22;
    
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        DrawText("Tasks queued", screenWidth - rightPanelWidth + 20, yPos, 12, GREEN);
    } else {
        DrawText("No tasks", screenWidth - rightPanelWidth + 20, yPos, 12, GRAY);
    }
}

void StaffGreenhouseScreen::DrawDayCounter() {
    int days = manager->GetDaysCounter();
    
    std::ostringstream dayStream;
    dayStream << "Day: " << days;
    
    DrawText(dayStream.str().c_str(), 20, 85, 24, GOLD);
}

void StaffGreenhouseScreen::DrawSchedulerOverlay() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});
    
    int overlayWidth = 650;
    int overlayHeight = 500;
    int overlayX = (screenWidth - overlayWidth) / 2;
    int overlayY = (screenHeight - overlayHeight) / 2;
    
    DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, Color{40, 40, 60, 255});
    DrawRectangleLinesEx(Rectangle{static_cast<float>(overlayX), static_cast<float>(overlayY), 
                                   static_cast<float>(overlayWidth), static_cast<float>(overlayHeight)}, 
                        3, GOLD);
    
    const char* title = "CARE SCHEDULER";
    int titleWidth = MeasureText(title, 24);
    DrawText(title, overlayX + (overlayWidth - titleWidth) / 2, overlayY + 20, 24, WHITE);
    
    int yPos = overlayY + 60;
    
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        DrawText("Next 3 Queued Tasks:", overlayX + 20, yPos, 18, YELLOW);
        yPos += 30;
        
        // Get all plants and identify which need care
        Greenhouse* greenhouse = manager->GetGreenhouse();
        if (greenhouse != nullptr) {
            std::vector<Plant*> allPlants = greenhouse->getAllPlants();
            
            std::vector<std::string> taskList;
            
            // Build list of tasks in order
            for (Plant* plant : allPlants) {
                if (plant != nullptr) {
                    if (plant->getWaterLevel() < 30) {
                        std::ostringstream task;
                        task << "Water " << plant->getName() << " (ID: " << plant->getID() 
                             << ") - Water: " << plant->getWaterLevel() << "%";
                        taskList.push_back(task.str());
                    }
                    if (plant->getNutrientLevel() < 30) {
                        std::ostringstream task;
                        task << "Fertilize " << plant->getName() << " (ID: " << plant->getID() 
                             << ") - Nutrients: " << plant->getNutrientLevel() << "%";
                        taskList.push_back(task.str());
                    }
                    if (plant->getSunlightExposure() < 40) {
                        std::ostringstream task;
                        task << "Adjust Sunlight for " << plant->getName() << " (ID: " << plant->getID() 
                             << ") - Light: " << plant->getSunlightExposure() << "%";
                        taskList.push_back(task.str());
                    }
                }
            }
            
            // Display next 3 tasks
            int tasksToShow = taskList.size() < 3 ? taskList.size() : 3;
            for (int i = 0; i < tasksToShow; i++) {
                std::ostringstream taskNum;
                taskNum << (i + 1) << ". " << taskList[i];
                
                // Color code by task type
                Color taskColor = WHITE;
                if (taskList[i].find("Water") != std::string::npos) {
                    taskColor = SKYBLUE;
                } else if (taskList[i].find("Fertilize") != std::string::npos) {
                    taskColor = Color{150, 255, 150, 255};
                } else if (taskList[i].find("Sunlight") != std::string::npos) {
                    taskColor = GOLD;
                }
                
                DrawText(taskNum.str().c_str(), overlayX + 30, yPos, 14, taskColor);
                yPos += 25;
            }
            
            yPos += 10;
            
            // Show total remaining
            if (taskList.size() > 3) {
                std::ostringstream remaining;
                remaining << "...and " << (taskList.size() - 3) << " more tasks";
                DrawText(remaining.str().c_str(), overlayX + 30, yPos, 14, LIGHTGRAY);
                yPos += 25;
            }
            
            // Show total count
            std::ostringstream totalText;
            totalText << "Total: " << taskList.size() << " tasks queued";
            DrawText(totalText.str().c_str(), overlayX + 30, yPos, 16, YELLOW);
            yPos += 15;
        }
        
        yPos += 20;
        DrawLine(overlayX + 20, yPos, overlayX + overlayWidth - 20, yPos, DARKGRAY);
        yPos += 25;
        
        DrawText("Choose an action:", overlayX + 20, yPos, 16, WHITE);
        yPos += 35;
    }
    
    // Draw buttons with proper spacing
    int buttonWidth = 240;
    int buttonHeight = 50;
    int buttonSpacing = 15;
    int buttonX = overlayX + (overlayWidth - buttonWidth) / 2;
    
    // Update button positions
    runAllButton.x = buttonX;
    runAllButton.y = yPos;
    runAllButton.width = buttonWidth;
    runAllButton.height = buttonHeight;
    
    runOneButton.x = buttonX;
    runOneButton.y = yPos + buttonHeight + buttonSpacing;
    runOneButton.width = buttonWidth;
    runOneButton.height = buttonHeight;
    
    closeOverlayButton.x = overlayX + overlayWidth / 2 - 60;
    closeOverlayButton.y = overlayY + overlayHeight - 55;
    closeOverlayButton.width = 120;
    closeOverlayButton.height = 40;
    
    // Draw Run All button
    Color runAllColor = runAllHovered ? DARKGREEN : GREEN;
    DrawRectangleRec(runAllButton, runAllColor);
    DrawRectangleLinesEx(runAllButton, 2, BLACK);
    const char* runAllText = "Run All & Advance Day";
    int runAllTextWidth = MeasureText(runAllText, 16);
    DrawText(runAllText,
             runAllButton.x + (runAllButton.width - runAllTextWidth) / 2,
             runAllButton.y + (runAllButton.height - 16) / 2,
             16,
             WHITE);
    
    // Draw Run One button
    Color runOneColor = runOneHovered ? ORANGE : Color{255, 150, 50, 255};
    DrawRectangleRec(runOneButton, runOneColor);
    DrawRectangleLinesEx(runOneButton, 2, BLACK);
    const char* runOneText = "Run One Task";
    int runOneTextWidth = MeasureText(runOneText, 16);
    DrawText(runOneText,
             runOneButton.x + (runOneButton.width - runOneTextWidth) / 2,
             runOneButton.y + (runOneButton.height - 16) / 2,
             16,
             WHITE);
    
    // Draw Close button
    Color closeColor = closeOverlayHovered ? DARKGRAY : GRAY;
    DrawRectangleRec(closeOverlayButton, closeColor);
    DrawRectangleLinesEx(closeOverlayButton, 2, BLACK);
    const char* closeText = schedulerTasksExecuted ? "Close & Advance Day" : "Close";
    int closeTextWidth = MeasureText(closeText, 14);
    DrawText(closeText,
             closeOverlayButton.x + (closeOverlayButton.width - closeTextWidth) / 2,
             closeOverlayButton.y + (closeOverlayButton.height - 14) / 2,
             14,
             WHITE);
    
    const char* escText = schedulerTasksExecuted ? "Press ESC to close & advance day" : "Press ESC to close";
    DrawText(escText, overlayX + 20, overlayY + overlayHeight - 20, 12, DARKGRAY);
}