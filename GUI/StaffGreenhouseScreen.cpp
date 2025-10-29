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
      backHovered(false),
      scheduledTasksCount(0) {
    
    gridRows = 6;
    gridCols = 8;
    
    InitializeLayout();
    InitializeButtons();
}

StaffGreenhouseScreen::~StaffGreenhouseScreen() {
    // Nothing to clean up
}

void StaffGreenhouseScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    // Layout: 20% | 60% | 20%
    leftPanelWidth = screenWidth / 5;
    middlePanelWidth = screenWidth * 3 / 5;
    rightPanelWidth = screenWidth / 5;
    
    // Calculate grid cell size
    int availableWidth = middlePanelWidth - 20;
    int availableHeight = screenHeight - 80;
    
    int cellSizeByWidth = availableWidth / gridCols;
    int cellSizeByHeight = availableHeight / gridRows;
    
    cellSize = (cellSizeByWidth < cellSizeByHeight) ? cellSizeByWidth : cellSizeByHeight;
    
    // Center the grid
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
    
    // Care action buttons (removed prune)
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
    
    // Transfer button (NEW)
    transferToSalesFloorButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 4 + 30),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight + 10)
    };
    
    // Scheduler button
    runSchedulerButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(screenHeight - 150),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    // Back button
    backButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(screenHeight - 90),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
}

void StaffGreenhouseScreen::Update() {
    // Update scheduled tasks count
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr) {
        scheduledTasksCount = scheduler->empty() ? 0 : 1;
    }
    
    UpdateGrid();
    UpdateButtons();
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
    
    // Update hover states (removed prune)
    waterHovered = CheckCollisionPointRec(mousePos, waterButton);
    fertilizeHovered = CheckCollisionPointRec(mousePos, fertilizeButton);
    sunlightHovered = CheckCollisionPointRec(mousePos, adjustSunlightButton);
    fullCareHovered = CheckCollisionPointRec(mousePos, performFullCareButton);
    transferHovered = CheckCollisionPointRec(mousePos, transferToSalesFloorButton);
    schedulerHovered = CheckCollisionPointRec(mousePos, runSchedulerButton);
    backHovered = CheckCollisionPointRec(mousePos, backButton);
    
    // Handle button clicks
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
        } else if (schedulerHovered) {
            HandleRunScheduler();
        } else if (backHovered) {
            manager->SwitchScreen(GameScreen::START);
        }
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
        selectedPlant->updateHealth();  // FIX: Update health after care
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
        selectedPlant->updateHealth();  // FIX: Update health after care
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
        selectedPlant->updateHealth();  // FIX: Update health after care
        std::cout << "[StaffGreenhouseScreen] Adjusted sunlight for plant " << selectedPlant->getID() 
                  << " - Sunlight: " << selectedPlant->getSunlightExposure() 
                  << "%, Health: " << selectedPlant->getHealthLevel() << "%" << std::endl;
    }
}

void StaffGreenhouseScreen::HandleFullCare() {
    if (selectedPlant == nullptr) return;
    
    selectedPlant->performCare();
    selectedPlant->updateHealth();  // FIX: Update health after care
    std::cout << "[StaffGreenhouseScreen] Performed full care on plant " << selectedPlant->getID() 
              << " - Health: " << selectedPlant->getHealthLevel() << "%" << std::endl;
}

void StaffGreenhouseScreen::HandleTransferToSalesFloor() {
    if (selectedPlant == nullptr) return;
    
    // Check if plant is ready for sale
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
    
    // Find empty spot on sales floor
    bool transferred = false;
    for (int row = 0; row < salesFloor->getRows() && !transferred; row++) {
        for (int col = 0; col < salesFloor->getColumns() && !transferred; col++) {
            if (salesFloor->isPositionEmpty(row, col)) {
                // Remove from greenhouse
                greenhouse->removePlant(selectedPlant);
                
                // Add to sales floor
                salesFloor->addPlantToDisplay(selectedPlant, row, col);
                
                std::cout << "[StaffGreenhouseScreen] Transferred " << selectedPlant->getName() 
                          << " (ID: " << selectedPlant->getID() 
                          << ") to sales floor at (" << row << "," << col << ")" << std::endl;
                
                // Clear selection
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

void StaffGreenhouseScreen::HandleRunScheduler() {
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        std::cout << "[StaffGreenhouseScreen] Running care scheduler..." << std::endl;
        scheduler->runAll();
        std::cout << "[StaffGreenhouseScreen] Scheduler tasks completed" << std::endl;
    } else {
        std::cout << "[StaffGreenhouseScreen] No tasks in scheduler" << std::endl;
    }
}

void StaffGreenhouseScreen::Draw() {
    ClearBackground(GRAY);
    
    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();
}

void StaffGreenhouseScreen::DrawLeftPanel() {
    DrawRectangle(0, 0, leftPanelWidth, screenHeight, Color{40, 40, 50, 255});
    DrawLine(leftPanelWidth, 0, leftPanelWidth, screenHeight, BLACK);
    
    int yPos = 20;
    
    DrawText("STAFF MODE", 20, yPos, 20, YELLOW);
    yPos += 35;
    
    DrawText("Greenhouse Manager", 20, yPos, 16, LIGHTGRAY);
    yPos += 45;
    
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
        
        std::string readyText = selectedPlant->isReadyForSale() ? "✓ Ready for Sale" : "✗ Still Growing";
        Color readyColor = selectedPlant->isReadyForSale() ? GREEN : ORANGE;
        DrawText(readyText.c_str(), 20, yPos, 14, readyColor);
        
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
    
    // Health bar at bottom
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
    
    // Water button
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
    
    // Fertilize button
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
    
    // Sunlight button
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
    
    // Full care button
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
    
    // Transfer button (NEW)
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
    
    // Run scheduler button
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
    
    // Back button
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
    int yPos = screenHeight - 180;
    
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