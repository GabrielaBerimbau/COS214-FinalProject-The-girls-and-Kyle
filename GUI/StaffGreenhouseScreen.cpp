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
      removeDeadHovered(false),
      backHovered(false),
      scheduledTasksCount(0),
      schedulerOverlayActive(false),
      schedulerTasksExecuted(false),
      runAllHovered(false),
      runOneHovered(false),
      closeOverlayHovered(false),
      displayedQueueSize(0) {

    gridRows = 6;
    gridCols = 8;

    InitializeLayout();
    InitializeButtons();

    // Initialize overlay button positions once
    InitializeOverlayButtons();
}

StaffGreenhouseScreen::~StaffGreenhouseScreen() {}

void StaffGreenhouseScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    leftPanelWidth   = screenWidth / 5;
    middlePanelWidth = screenWidth * 3 / 5;
    rightPanelWidth  = screenWidth / 5;

    int availableWidth  = middlePanelWidth - 20;
    int availableHeight = screenHeight - 80;

    int cellSizeByWidth  = availableWidth / gridCols;
    int cellSizeByHeight = availableHeight / gridRows;

    cellSize = (cellSizeByWidth < cellSizeByHeight) ? cellSizeByWidth : cellSizeByHeight;

    int gridTotalWidth  = cellSize * gridCols;
    int gridTotalHeight = cellSize * gridRows;

    gridStartX = leftPanelWidth + (middlePanelWidth - gridTotalWidth) / 2;
    gridStartY = 60 + (screenHeight - 60 - gridTotalHeight) / 2;
}

void StaffGreenhouseScreen::InitializeButtons() {
    int buttonWidth   = rightPanelWidth - 40;
    int buttonHeight  = 40;
    int buttonSpacing = 10;
    int buttonX       = screenWidth - rightPanelWidth + 20;
    int startY        = 60;

    waterButton = Rectangle{ (float)buttonX, (float)startY, (float)buttonWidth, (float)buttonHeight };
    fertilizeButton = Rectangle{ (float)buttonX, (float)(startY + (buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight };
    adjustSunlightButton = Rectangle{ (float)buttonX, (float)(startY + (buttonHeight + buttonSpacing) * 2), (float)buttonWidth, (float)buttonHeight };
    performFullCareButton = Rectangle{ (float)buttonX, (float)(startY + (buttonHeight + buttonSpacing) * 3 + 10), (float)buttonWidth, (float)(buttonHeight + 10) };
    transferToSalesFloorButton = Rectangle{ (float)buttonX, (float)(startY + (buttonHeight + buttonSpacing) * 4 + 30), (float)buttonWidth, (float)(buttonHeight + 10) };
    removeDeadPlantButton = Rectangle{ (float)buttonX, (float)(startY + (buttonHeight + buttonSpacing) * 5 + 50), (float)buttonWidth, (float)(buttonHeight + 10) };
    advanceDayButton = Rectangle{ (float)buttonX, (float)(screenHeight - 200), (float)buttonWidth, (float)(buttonHeight + 10) };
    runSchedulerButton = Rectangle{ (float)buttonX, (float)(screenHeight - 130), (float)buttonWidth, (float)buttonHeight };
    backButton = Rectangle{ (float)buttonX, (float)(screenHeight - 70), (float)buttonWidth, (float)buttonHeight };

    viewSalesFloorButton = Rectangle{
    (float)buttonX,
    (float)(startY + (buttonHeight + buttonSpacing) * 6 + 60),  // below your other actions
    (float)buttonWidth,
    (float)(buttonHeight + 10)
};
}

// Initialize overlay buttons once (moved a bit higher so they don't crowd "Close")
void StaffGreenhouseScreen::InitializeOverlayButtons() {
    int overlayWidth  = 650;
    int overlayHeight = 500;
    int overlayX      = (screenWidth - overlayWidth) / 2;
    int overlayY      = (screenHeight - overlayHeight) / 2;

    int buttonWidth   = 240;
    int buttonHeight  = 50;
    int buttonSpacing = 15;

    // Move the stack up a bit to avoid overlapping Close/ESC area
    int yPos = overlayY + 300; // was 350

    int buttonX = overlayX + (overlayWidth - buttonWidth) / 2;

    runAllButton  = Rectangle{ (float)buttonX, (float)yPos, (float)buttonWidth, (float)buttonHeight };
    runOneButton  = Rectangle{ (float)buttonX, (float)(yPos + buttonHeight + buttonSpacing), (float)buttonWidth, (float)buttonHeight };
    closeOverlayButton = Rectangle{ (float)(overlayX + overlayWidth / 2 - 60), (float)(overlayY + overlayHeight - 55), 120.0f, 40.0f };
}

bool StaffGreenhouseScreen::IsPlantDead(Plant* plant) const {
    if (plant == nullptr || plant->getState() == nullptr) return false;
    return plant->getState()->getStateName() == "Dead";
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

        int col = (int)((mousePos.x - gridStartX) / cellSize);
        int row = (int)((mousePos.y - gridStartY) / cellSize);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            HandlePlantSelection(row, col);
        }
    }
}

void StaffGreenhouseScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();

    waterHovered      = CheckCollisionPointRec(mousePos, waterButton);
    fertilizeHovered  = CheckCollisionPointRec(mousePos, fertilizeButton);
    sunlightHovered   = CheckCollisionPointRec(mousePos, adjustSunlightButton);
    fullCareHovered   = CheckCollisionPointRec(mousePos, performFullCareButton);
    transferHovered   = CheckCollisionPointRec(mousePos, transferToSalesFloorButton);
    removeDeadHovered = CheckCollisionPointRec(mousePos, removeDeadPlantButton);
    advanceDayHovered = CheckCollisionPointRec(mousePos, advanceDayButton);
    schedulerHovered  = CheckCollisionPointRec(mousePos, runSchedulerButton);
    backHovered       = CheckCollisionPointRec(mousePos, backButton);
    viewSalesFloorHovered = CheckCollisionPointRec(mousePos, viewSalesFloorButton);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        bool plantIsDead = IsPlantDead(selectedPlant);

        if (waterHovered && selectedPlant != nullptr && !plantIsDead) {
            HandleWaterPlant();
        } else if (fertilizeHovered && selectedPlant != nullptr && !plantIsDead) {
            HandleFertilizePlant();
        } else if (sunlightHovered && selectedPlant != nullptr && !plantIsDead) {
            HandleAdjustSunlight();
        } else if (fullCareHovered && selectedPlant != nullptr && !plantIsDead) {
            HandleFullCare();
        } else if (transferHovered && selectedPlant != nullptr && !plantIsDead) {
            HandleTransferToSalesFloor();
        } else if (removeDeadHovered && selectedPlant != nullptr && plantIsDead) {
            HandleRemoveDeadPlant();
        } else if (advanceDayHovered) {
            HandleAdvanceDay();
        } else if (schedulerHovered) {
            HandleRunScheduler();
        } else if (backHovered) {
            manager->SwitchScreen(GameScreen::START);
        }else if (viewSalesFloorHovered) {
            manager->SwitchScreen(GameScreen::STAFF_SALES_FLOOR);
        }
    }
}

void StaffGreenhouseScreen::UpdateSchedulerOverlay() {
    Vector2 mousePos = GetMousePosition();

    runAllHovered      = CheckCollisionPointRec(mousePos, runAllButton);
    runOneHovered      = CheckCollisionPointRec(mousePos, runOneButton);
    closeOverlayHovered= CheckCollisionPointRec(mousePos, closeOverlayButton);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (runAllHovered) {
            HandleRunAllScheduled();
        } else if (runOneHovered) {
            HandleRunOneScheduled();
        } else if (closeOverlayHovered) {
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
                  << " at (" << row << "," << col << ")";
        if (IsPlantDead(plant)) std::cout << " - DEAD PLANT";
        std::cout << std::endl;
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

void StaffGreenhouseScreen::HandleRemoveDeadPlant() {
    if (selectedPlant == nullptr) return;

    if (!IsPlantDead(selectedPlant)) {
        std::cout << "[StaffGreenhouseScreen] Plant is not dead, cannot remove!" << std::endl;
        return;
    }

    Greenhouse* greenhouse = manager->GetGreenhouse();
    if (greenhouse == nullptr) return;

    std::cout << "[StaffGreenhouseScreen] Removing dead plant " << selectedPlant->getName()
              << " (ID: " << selectedPlant->getID() << ") from greenhouse" << std::endl;

    Plant* deadPlant = greenhouse->removePlantAt(selectedRow, selectedCol);
    if (deadPlant != nullptr) {
        delete deadPlant;
        std::cout << "[StaffGreenhouseScreen] Dead plant removed and disposed of" << std::endl;
    }

    selectedPlant = nullptr;
    selectedRow = -1;
    selectedCol = -1;
}

void StaffGreenhouseScreen::HandleAdvanceDay() {
    std::cout << "[StaffGreenhouseScreen] Advancing day manually..." << std::endl;
    manager->PerformDailyUpdate();
    std::cout << "[StaffGreenhouseScreen] Day advanced to: " << manager->GetDaysCounter() << std::endl;
}

void StaffGreenhouseScreen::HandleRunScheduler() {
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        schedulerOverlayActive  = true;
        schedulerTasksExecuted  = false;
        displayedQueueSize      = CountQueuedTasks(); // initial count

        std::cout << "[StaffGreenhouseScreen] Opening scheduler overlay with "
                  << displayedQueueSize << " tasks..." << std::endl;
    } else {
        std::cout << "[StaffGreenhouseScreen] No tasks in scheduler" << std::endl;
    }
}

int StaffGreenhouseScreen::CountQueuedTasks() {
    CareScheduler* scheduler = manager->GetCareScheduler();
    Greenhouse* greenhouse   = manager->GetGreenhouse();
    if (scheduler == nullptr || greenhouse == nullptr || scheduler->empty()) return 0;

    int taskCount = 0;
    std::vector<Plant*> allPlants = greenhouse->getAllPlants();
    for (Plant* plant : allPlants) {
        if (plant != nullptr && !IsPlantDead(plant)) {
            if (plant->getWaterLevel() < 30)      taskCount++;
            if (plant->getNutrientLevel() < 30)   taskCount++;
            if (plant->getSunlightExposure() < 40)taskCount++;
        }
    }
    return taskCount;
}

void StaffGreenhouseScreen::HandleRunAllScheduled() {
    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        std::cout << "[StaffGreenhouseScreen] Running all scheduled tasks..." << std::endl;
        scheduler->runAll();
        schedulerTasksExecuted = true;
        displayedQueueSize = 0;

        std::cout << "[StaffGreenhouseScreen] All tasks completed. Close overlay to advance day." << std::endl;
        schedulerOverlayActive = false;

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

        displayedQueueSize = CountQueuedTasks();

        std::cout << "[StaffGreenhouseScreen] Task executed. Remaining tasks: "
                  << displayedQueueSize << std::endl;

        if (scheduler->empty() || displayedQueueSize == 0) {
            std::cout << "[StaffGreenhouseScreen] All tasks completed" << std::endl;
            schedulerOverlayActive = false;

            manager->PerformDailyUpdate();
            std::cout << "[StaffGreenhouseScreen] Day advanced to: " << manager->GetDaysCounter() << std::endl;
            schedulerTasksExecuted = false;
        }
    }
}

void StaffGreenhouseScreen::Draw() {
    ClearBackground(Color{216, 228, 220, 255}); // Soft sage green

    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();

    if (schedulerOverlayActive) {
        DrawSchedulerOverlay();
    }
}

void StaffGreenhouseScreen::DrawLeftPanel() {
    DrawRectangle(0, 0, leftPanelWidth, screenHeight, Color{230, 224, 237, 255}); // Soft lavender
    DrawLine(leftPanelWidth, 0, leftPanelWidth, screenHeight, Color{85, 107, 95, 255}); // Dark forest green

    int yPos = 20;

    DrawText("STAFF MODE", 20, yPos, 20, Color{235, 186, 170, 255}); // Warm terracotta
    yPos += 35;

    DrawText("Greenhouse Manager", 20, yPos, 16, Color{120, 140, 125, 255}); // Medium sage
    yPos += 30;

    DrawDayCounter();
    yPos += 60;

    DrawLine(20, yPos, leftPanelWidth - 20, yPos, Color{85, 107, 95, 255});
    yPos += 15;

    if (selectedPlant != nullptr) {
        bool isDead = IsPlantDead(selectedPlant);

        if (isDead) {
            DrawText("DEAD PLANT", 20, yPos, 18, Color{200, 100, 100, 255});
            yPos += 30;
        }

        DrawText("SELECTED PLANT", 20, yPos, 18, isDead ? Color{200, 100, 100, 255} : Color{235, 186, 170, 255});
        yPos += 30;

        std::string plantNameText = selectedPlant->getName();
        DrawText(plantNameText.c_str(), 20, yPos, 16, Color{85, 107, 95, 255});
        yPos += 25;

        std::string idText = "ID: " + selectedPlant->getID();
        DrawText(idText.c_str(), 20, yPos, 13, Color{120, 140, 125, 255});
        yPos += 25;

        std::string stateText = "State: " + selectedPlant->getState()->getStateName();
        Color stateColor = isDead ? Color{200, 100, 100, 255} : Color{120, 140, 125, 255};
        DrawText(stateText.c_str(), 20, yPos, 13, stateColor);
        yPos += 25;

        std::ostringstream ageStream;
        ageStream << "Age: " << selectedPlant->getAge() << " days";
        DrawText(ageStream.str().c_str(), 20, yPos, 13, Color{120, 140, 125, 255});
        yPos += 30;

        DrawLine(20, yPos, leftPanelWidth - 20, yPos, Color{200, 210, 205, 255});
        yPos += 15;

        if (isDead) {
            DrawText("This plant has died.", 20, yPos, 14, Color{200, 100, 100, 255});
            yPos += 22;
            DrawText("Use 'Remove Dead'", 20, yPos, 14, Color{235, 186, 170, 255});
            yPos += 18;
            DrawText("button to dispose", 20, yPos, 14, Color{235, 186, 170, 255});
            yPos += 18;
            DrawText("of it.", 20, yPos, 14, Color{235, 186, 170, 255});
        } else {
            DrawText("PLANT STATUS:", 20, yPos, 14, Color{60, 60, 60, 255});
            yPos += 25;

            std::ostringstream healthStream;
            healthStream << "Health: " << selectedPlant->getHealthLevel() << "%";
            Color healthColor = selectedPlant->getHealthLevel() > 70 ? Color{120, 165, 120, 255} :
                                selectedPlant->getHealthLevel() > 40 ? Color{235, 186, 170, 255} : Color{200, 100, 100, 255};
            DrawText(healthStream.str().c_str(), 20, yPos, 14, healthColor);
            yPos += 22;

            std::ostringstream waterStream;
            waterStream << "Water: " << selectedPlant->getWaterLevel() << "%";
            DrawText(waterStream.str().c_str(), 20, yPos, 14, Color{60, 60, 60, 255});
            yPos += 22;

            std::ostringstream nutrientStream;
            nutrientStream << "Nutrients: " << selectedPlant->getNutrientLevel() << "%";
            Color nutrientColor = selectedPlant->getNutrientLevel() > 50 ? Color{120, 165, 120, 255} :
                                  selectedPlant->getNutrientLevel() > 25 ? Color{235, 186, 170, 255} : Color{200, 100, 100, 255};
            DrawText(nutrientStream.str().c_str(), 20, yPos, 14, nutrientColor);
            yPos += 22;

            std::ostringstream sunStream;
            sunStream << "Sunlight: " << selectedPlant->getSunlightExposure() << "%";
            DrawText(sunStream.str().c_str(), 20, yPos, 14, Color{60, 60, 60, 255});
            yPos += 30;
        }
    } else {
        DrawText("INSTRUCTIONS:", 20, yPos, 16, Color{235, 186, 170, 255});
        yPos += 30;

        DrawText("1. Click a plant to select", 20, yPos, 13, Color{85, 107, 95, 255});
        yPos += 22;
        DrawText("2. Use care buttons", 20, yPos, 13, Color{85, 107, 95, 255});
        yPos += 22;
        DrawText("3. Monitor health status", 20, yPos, 13, Color{85, 107, 95, 255});
        yPos += 22;
        DrawText("4. Transfer when ready", 20, yPos, 13, Color{85, 107, 95, 255});
        yPos += 18;
        DrawText("   for sale", 20, yPos, 13, Color{85, 107, 95, 255});
        yPos += 22;
        DrawText("5. Remove dead plants", 20, yPos, 13, Color{85, 107, 95, 255});
    }
}

void StaffGreenhouseScreen::DrawMiddlePanel() {
    DrawRectangle(leftPanelWidth, 0, middlePanelWidth, screenHeight, Color{206, 237, 223, 255}); // Soft mint
    DrawLine(leftPanelWidth + middlePanelWidth, 0, leftPanelWidth + middlePanelWidth, screenHeight, Color{85, 107, 95, 255});

    const char* header = "STAFF GREENHOUSE - PLANT CARE";
    int headerSize = 22;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header,
             leftPanelWidth + (middlePanelWidth - headerWidth) / 2,
             20,
             headerSize,
             Color{85, 107, 95, 255});

    DrawGrid();
}

void StaffGreenhouseScreen::DrawRightPanel() {
    DrawRectangle(leftPanelWidth + middlePanelWidth, 0, rightPanelWidth, screenHeight, Color{255, 236, 214, 255}); // Peachy cream

    const char* header = "CARE ACTIONS";
    int headerSize = 18;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header,
             screenWidth - rightPanelWidth + (rightPanelWidth - headerWidth) / 2,
             20,
             headerSize,
             Color{85, 107, 95, 255});

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

            Plant* plant = greenhouse->getPlantAt(row, col);
            bool isDead = IsPlantDead(plant);

            Color cellColor = Color{245, 250, 247, 255};
            if (row == selectedRow && col == selectedCol) {
                cellColor = isDead ? Color{255, 220, 220, 255} : Color{255, 247, 204, 255};
            } else if (isDead) {
                cellColor = Color{245, 215, 215, 255};
            }

            DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, cellColor);
            DrawRectangleLines(x + 1, y + 1, cellSize - 2, cellSize - 2, isDead ? Color{200, 100, 100, 255} : Color{200, 210, 205, 255});

            if (plant != nullptr) {
                DrawPlantInCell(plant, row, col);
                if (isDead) {
                    const char* skullIcon = "X";
                    int iconWidth = MeasureText(skullIcon, 30);
                    DrawText(skullIcon, x + (cellSize - iconWidth) / 2, y + cellSize / 2 - 15, 30, Color{200, 100, 100, 255});
                }
            }
        }
    }
}

void StaffGreenhouseScreen::DrawPlantInCell(Plant* plant, int row, int col) {
    int x = gridStartX + col * cellSize;
    int y = gridStartY + row * cellSize;

    bool isDead = IsPlantDead(plant);
    Texture2D plantTexture = manager->GetPlantTexture(plant->getName());

    if (plantTexture.id != 0) {
        int maxSize = cellSize - 20;
        float scale = (float)maxSize / plantTexture.height;

        int scaledWidth  = (int)(plantTexture.width * scale);
        int scaledHeight = (int)(plantTexture.height * scale);

        int texX = x + (cellSize - scaledWidth) / 2;
        int texY = y + (cellSize - scaledHeight) / 2 - 5;

        Color tint = isDead ? Color{100, 100, 100, 255} : WHITE;
        DrawTextureEx(plantTexture, Vector2{(float)texX, (float)texY}, 0.0f, scale, tint);
    } else {
        const char* name = plant->getName().c_str();
        int nameWidth = MeasureText(name, 10);
        Color textColor = isDead ? Color{200, 100, 100, 255} : Color{85, 107, 95, 255};
        DrawText(name, x + (cellSize - nameWidth) / 2, y + cellSize / 2 - 10, 10, textColor);
    }

    if (!isDead) {
        int healthBarWidth  = cellSize - 10;
        int healthBarHeight = 4;
        int healthBarX = x + 5;
        int healthBarY = y + cellSize - 8;

        int health = plant->getHealthLevel();
        Color healthColor = health > 70 ? Color{120, 165, 120, 255} :
                            health > 40 ? Color{235, 186, 170, 255} : Color{200, 100, 100, 255};

        DrawRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight, Color{200, 210, 205, 255});
        DrawRectangle(healthBarX, healthBarY, (healthBarWidth * health) / 100, healthBarHeight, healthColor);
    }
}

void StaffGreenhouseScreen::DrawButtons() {
    bool hasSelection = selectedPlant != nullptr;
    bool isDead = IsPlantDead(selectedPlant);
    bool canCare = hasSelection && !isDead;

    // Pastel palette for nicer, distinct buttons
    // Water: soft teal
    Color waterBase  = Color{120, 200, 220, 255};
    Color waterHover = Color{100, 180, 200, 255};
    Color waterColor = canCare ? (waterHovered ? waterHover : waterBase) : Color{180, 180, 180, 255};
    DrawRectangleRec(waterButton, waterColor);
    DrawRectangleLinesEx(waterButton, 2, BLACK);
    const char* waterText = "Water";
    int waterTextWidth = MeasureText(waterText, 16);
    DrawText(waterText, waterButton.x + (waterButton.width - waterTextWidth) / 2,
             waterButton.y + (waterButton.height - 16) / 2, 16, canCare ? BLACK : DARKGRAY);

    // Fertilize: soft green
    Color fertBase  = Color{150, 210, 160, 255};
    Color fertHover = Color{130, 190, 140, 255};
    Color fertColor = canCare ? (fertilizeHovered ? fertHover : fertBase) : Color{180, 180, 180, 255};
    DrawRectangleRec(fertilizeButton, fertColor);
    DrawRectangleLinesEx(fertilizeButton, 2, BLACK);
    const char* fertText = "Fertilize";
    int fertTextWidth = MeasureText(fertText, 16);
    DrawText(fertText, fertilizeButton.x + (fertilizeButton.width - fertTextWidth) / 2,
             fertilizeButton.y + (fertilizeButton.height - 16) / 2, 16, BLACK);

    // Sunlight: light butter
    Color sunBase  = Color{255, 235, 150, 255};
    Color sunHover = Color{255, 225, 120, 255};
    Color sunColor = canCare ? (sunlightHovered ? sunHover : sunBase) : Color{180, 180, 180, 255};
    DrawRectangleRec(adjustSunlightButton, sunColor);
    DrawRectangleLinesEx(adjustSunlightButton, 2, BLACK);
    const char* sunText = "Sunlight";
    int sunTextWidth = MeasureText(sunText, 16);
    DrawText(sunText, adjustSunlightButton.x + (adjustSunlightButton.width - sunTextWidth) / 2,
             adjustSunlightButton.y + (adjustSunlightButton.height - 16) / 2, 16, BLACK);

    // Full care: soft lavender
    Color fullBase  = Color{190, 160, 210, 255};
    Color fullHover = Color{175, 145, 195, 255};
    Color fullCareColor = canCare ? (fullCareHovered ? fullHover : fullBase) : Color{180, 180, 180, 255};
    DrawRectangleRec(performFullCareButton, fullCareColor);
    DrawRectangleLinesEx(performFullCareButton, 3, BLACK);
    const char* fullCareText = "FULL CARE";
    int fullCareTextWidth = MeasureText(fullCareText, 18);
    DrawText(fullCareText, performFullCareButton.x + (performFullCareButton.width - fullCareTextWidth) / 2,
             performFullCareButton.y + (performFullCareButton.height - 18) / 2, 18, WHITE);

    // Transfer: soft sky blue
    bool canTransfer = canCare && selectedPlant->isReadyForSale();
    Color transBase  = Color{140, 180, 230, 255};
    Color transHover = Color{120, 160, 210, 255};
    Color transferColor = canTransfer ? (transferHovered ? transHover : transBase) : Color{180, 180, 180, 255};
    DrawRectangleRec(transferToSalesFloorButton, transferColor);
    DrawRectangleLinesEx(transferToSalesFloorButton, 3, BLACK);
    const char* transferText = "To Sales Floor";
    int transferTextWidth = MeasureText(transferText, 16);
    DrawText(transferText, transferToSalesFloorButton.x + (transferToSalesFloorButton.width - transferTextWidth) / 2,
             transferToSalesFloorButton.y + (transferToSalesFloorButton.height - 16) / 2, 16,
             canTransfer ? WHITE : DARKGRAY);

    // Remove dead: soft red
    bool canRemove = hasSelection && isDead;
    Color remBase  = Color{230, 120, 120, 255};
    Color remHover = Color{210, 90, 90, 255};
    Color removeColor = canRemove ? (removeDeadHovered ? remHover : remBase) : Color{180, 180, 180, 255};
    DrawRectangleRec(removeDeadPlantButton, removeColor);
    DrawRectangleLinesEx(removeDeadPlantButton, 3, BLACK);
    const char* removeText = "Remove Dead";
    int removeTextWidth = MeasureText(removeText, 15);
    DrawText(removeText, removeDeadPlantButton.x + (removeDeadPlantButton.width - removeTextWidth) / 2,
             removeDeadPlantButton.y + (removeDeadPlantButton.height - 15) / 2, 15,
             canRemove ? WHITE : DARKGRAY);

    // Advance day: keep your yellow scheme
    Color advanceBase  = Color{255, 150, 0, 255};
    Color advanceHover = Color{255, 200, 0, 255};
    Color advanceDayColor = advanceDayHovered ? advanceHover : advanceBase;
    DrawRectangleRec(advanceDayButton, advanceDayColor);
    DrawRectangleLinesEx(advanceDayButton, 3, BLACK);
    const char* advanceDayText = "Advance Day";
    int advanceDayTextWidth = MeasureText(advanceDayText, 16);
    DrawText(advanceDayText, advanceDayButton.x + (advanceDayButton.width - advanceDayTextWidth) / 2,
             advanceDayButton.y + (advanceDayButton.height - 16) / 2, 16, BLACK);

    // Run Scheduler: SAME yellow as Advance Day
    Color schedulerColor = schedulerHovered ? advanceHover : advanceBase;
    DrawRectangleRec(runSchedulerButton, schedulerColor);
    DrawRectangleLinesEx(runSchedulerButton, 2, BLACK);
    const char* schedulerText = "Run Scheduler";
    int schedulerTextWidth = MeasureText(schedulerText, 15);
    DrawText(schedulerText, runSchedulerButton.x + (runSchedulerButton.width - schedulerTextWidth) / 2,
             runSchedulerButton.y + (runSchedulerButton.height - 15) / 2, 15, BLACK);

    // Back: neutral grey
    Color backColor = backHovered ? DARKGRAY : Color{100, 100, 100, 255};
    DrawRectangleRec(backButton, backColor);
    DrawRectangleLinesEx(backButton, 2, BLACK);
    const char* backText = "Back";
    int backTextWidth = MeasureText(backText, 16);
    DrawText(backText, backButton.x + (backButton.width - backTextWidth) / 2,
             backButton.y + (backButton.height - 16) / 2, 16, WHITE);


    Color viewBase  = Color{140, 180, 230, 255};
    Color viewHover = Color{120, 160, 210, 255};
    Color viewColor = viewSalesFloorHovered ? viewHover : viewBase;
    DrawRectangleRec(viewSalesFloorButton, viewColor);
    DrawRectangleLinesEx(viewSalesFloorButton, 3, BLACK);
    const char* viewText = "View Sales Floor";
    int viewW = MeasureText(viewText, 16);
    DrawText(viewText,
            viewSalesFloorButton.x + (viewSalesFloorButton.width - viewW)/2,
            viewSalesFloorButton.y + (viewSalesFloorButton.height - 16)/2,
            16, WHITE);
}

void StaffGreenhouseScreen::DrawSchedulerInfo() {
    int yPos = screenHeight - 250;

    DrawLine(screenWidth - rightPanelWidth + 20, yPos, screenWidth - 20, yPos, Color{200, 210, 205, 255});
    yPos += 10;

    DrawText("SCHEDULER:", screenWidth - rightPanelWidth + 20, yPos, 14, Color{60, 60, 60, 255});
    yPos += 22;

    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        DrawText("Tasks queued", screenWidth - rightPanelWidth + 20, yPos, 12, Color{60, 60, 60, 255});
    } else {
        DrawText("No tasks", screenWidth - rightPanelWidth + 20, yPos, 12, Color{120, 140, 125, 255});
    }
}

void StaffGreenhouseScreen::DrawDayCounter() {
    int days = manager->GetDaysCounter();
    std::ostringstream dayStream;
    dayStream << "Day: " << days;
    DrawText(dayStream.str().c_str(), 20, 85, 24, Color{235, 186, 170, 255});
}

void StaffGreenhouseScreen::DrawSchedulerOverlay() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});

    int overlayWidth  = 650;
    int overlayHeight = 500;
    int overlayX      = (screenWidth - overlayWidth) / 2;
    int overlayY      = (screenHeight - overlayHeight) / 2;

    DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, Color{230, 224, 237, 255}); // Soft lavender
    DrawRectangleLinesEx(Rectangle{(float)overlayX, (float)overlayY, (float)overlayWidth, (float)overlayHeight},
                         3, Color{235, 186, 170, 255}); // Terracotta border

    const char* title = "CARE SCHEDULER";
    int titleWidth = MeasureText(title, 24);
    DrawText(title, overlayX + (overlayWidth - titleWidth) / 2, overlayY + 20, 24, Color{50, 70, 60, 255}); // darker green

    int yPos = overlayY + 60;

    // Darker, clearer text
    Color textMain = Color{40, 40, 40, 255};
    Color textSub  = Color{70, 70, 70, 255};

    if (displayedQueueSize > 0) {
        DrawText("Next 3 Queued Tasks:", overlayX + 20, yPos, 18, textMain);
        yPos += 30;

        Greenhouse* greenhouse = manager->GetGreenhouse();
        if (greenhouse != nullptr) {
            std::vector<Plant*> allPlants = greenhouse->getAllPlants();
            std::vector<std::string> taskList;

            for (Plant* plant : allPlants) {
                if (plant != nullptr && !IsPlantDead(plant)) {
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

            int tasksToShow = (int)taskList.size() < 3 ? (int)taskList.size() : 3;
            for (int i = 0; i < tasksToShow; i++) {
                std::ostringstream taskNum;
                taskNum << (i + 1) << ". " << taskList[i];
                DrawText(taskNum.str().c_str(), overlayX + 30, yPos, 14, textSub);
                yPos += 25;
            }

            yPos += 10;

            if (taskList.size() > 3) {
                std::ostringstream remaining;
                remaining << "...and " << (taskList.size() - 3) << " more tasks";
                DrawText(remaining.str().c_str(), overlayX + 30, yPos, 14, Color{100, 100, 100, 255});
                yPos += 25;
            }

            std::ostringstream totalText;
            totalText << "Total: " << displayedQueueSize << " tasks queued";
            DrawText(totalText.str().c_str(), overlayX + 30, yPos, 16, textMain);
            yPos += 15;
        }

        yPos += 20;
        DrawLine(overlayX + 20, yPos, overlayX + overlayWidth - 20, yPos, DARKGRAY);
        yPos += 25;

        DrawText("Choose an action:", overlayX + 20, yPos, 16, textMain);
        yPos += 35;
    } else {
        DrawText("No tasks in queue!", overlayX + 30, yPos, 18, textMain);
        yPos += 40;
        DrawText("All plants are well cared for.", overlayX + 30, yPos, 14, textSub);
    }

    // Overlay buttons with lighter colors
    Color runAllBase  = Color{160, 220, 160, 255};
    Color runAllHover = Color{130, 200, 130, 255};
    Color runAllColor = runAllHovered ? runAllHover : runAllBase;
    DrawRectangleRec(runAllButton, runAllColor);
    DrawRectangleLinesEx(runAllButton, 2, BLACK);
    const char* runAllText = "Run All & Advance Day";
    int runAllTextWidth = MeasureText(runAllText, 16);
    DrawText(runAllText, runAllButton.x + (runAllButton.width - runAllTextWidth) / 2,
             runAllButton.y + (runAllButton.height - 16) / 2, 16, BLACK);

    Color runOneBase  = Color{255, 200, 120, 255};
    Color runOneHover = Color{255, 180, 90, 255};
    Color runOneColor = runOneHovered ? runOneHover : runOneBase;
    DrawRectangleRec(runOneButton, runOneColor);
    DrawRectangleLinesEx(runOneButton, 2, BLACK);
    const char* runOneText = "Run One Task";
    int runOneTextWidth = MeasureText(runOneText, 16);
    DrawText(runOneText, runOneButton.x + (runOneButton.width - runOneTextWidth) / 2,
             runOneButton.y + (runOneButton.height - 16) / 2, 16, BLACK);

    Color closeColor = closeOverlayHovered ? DARKGRAY : GRAY;
    DrawRectangleRec(closeOverlayButton, closeColor);
    DrawRectangleLinesEx(closeOverlayButton, 2, BLACK);
    const char* closeText = schedulerTasksExecuted ? "Close & Advance Day" : "Close";
    int closeTextWidth = MeasureText(closeText, 14);
    DrawText(closeText, closeOverlayButton.x + (closeOverlayButton.width - closeTextWidth) / 2,
             closeOverlayButton.y + (closeOverlayButton.height - 14) / 2, 14, WHITE);

    DrawText(schedulerTasksExecuted ? "Press ESC to close & advance day" : "Press ESC to close",
             overlayX + 20, overlayY + overlayHeight - 20, 12, DARKGRAY);
}
