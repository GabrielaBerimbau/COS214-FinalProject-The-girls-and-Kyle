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
    Color bgColor = manager->IsAlternativeColors()
        ? Color{30, 30, 35, 255}     // Dark mode
        : Color{216, 228, 220, 255}; // Soft sage green
    ClearBackground(bgColor);

    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();

    if (schedulerOverlayActive) {
        DrawSchedulerOverlay();
    }
}

void StaffGreenhouseScreen::DrawLeftPanel() {
    Color panelBg = manager->IsAlternativeColors()
        ? Color{45, 45, 50, 255}     // Dark panel
        : Color{230, 224, 237, 255}; // Soft lavender
    DrawRectangle(0, 0, leftPanelWidth, screenHeight, panelBg);

    Color borderColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint border
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawLine(leftPanelWidth, 0, leftPanelWidth, screenHeight, borderColor);

    int yPos = 20;

    Color headerColor = manager->IsAlternativeColors()
        ? Color{255, 180, 160, 255}  // Light terracotta
        : Color{235, 186, 170, 255}; // Warm terracotta
    DrawText("STAFF MODE", 20, yPos, 20, headerColor);
    yPos += 35;

    Color subtitleColor = manager->IsAlternativeColors()
        ? Color{180, 200, 190, 255}  // Light grey-green
        : Color{120, 140, 125, 255}; // Medium sage
    DrawText("Greenhouse Manager", 20, yPos, 16, subtitleColor);
    yPos += 30;

    DrawDayCounter();
    yPos += 60;

    DrawLine(20, yPos, leftPanelWidth - 20, yPos, borderColor);
    yPos += 15;

    if (selectedPlant != nullptr) {
        bool isDead = IsPlantDead(selectedPlant);

        Color deadColor = manager->IsAlternativeColors()
            ? Color{255, 140, 140, 255}  // Lighter red for dark mode
            : Color{200, 100, 100, 255}; // Red for light mode
        Color aliveHeaderColor = manager->IsAlternativeColors()
            ? Color{255, 180, 160, 255}  // Light terracotta
            : Color{235, 186, 170, 255}; // Warm terracotta
        Color textColor = manager->IsAlternativeColors()
            ? Color{200, 200, 200, 255}  // Light grey
            : Color{85, 107, 95, 255};   // Dark forest green
        Color textSecondary = manager->IsAlternativeColors()
            ? Color{160, 160, 160, 255}  // Medium grey
            : Color{120, 140, 125, 255}; // Medium sage

        if (isDead) {
            DrawText("DEAD PLANT", 20, yPos, 18, deadColor);
            yPos += 30;
        }

        DrawText("SELECTED PLANT", 20, yPos, 18, isDead ? deadColor : aliveHeaderColor);
        yPos += 30;

        std::string plantNameText = selectedPlant->getName();
        DrawText(plantNameText.c_str(), 20, yPos, 16, textColor);
        yPos += 25;

        std::string idText = "ID: " + selectedPlant->getID();
        DrawText(idText.c_str(), 20, yPos, 13, textSecondary);
        yPos += 25;

        std::string stateText = "State: " + selectedPlant->getState()->getStateName();
        Color stateColor = isDead ? deadColor : textSecondary;
        DrawText(stateText.c_str(), 20, yPos, 13, stateColor);
        yPos += 25;

        std::ostringstream ageStream;
        ageStream << "Age: " << selectedPlant->getAge() << " days";
        DrawText(ageStream.str().c_str(), 20, yPos, 13, textSecondary);
        yPos += 30;

        Color lineColor = manager->IsAlternativeColors()
            ? Color{80, 80, 85, 255}     // Dark grey line
            : Color{200, 210, 205, 255}; // Light grey line
        DrawLine(20, yPos, leftPanelWidth - 20, yPos, lineColor);
        yPos += 15;

        if (isDead) {
            DrawText("This plant has died.", 20, yPos, 14, deadColor);
            yPos += 22;
            DrawText("Use 'Remove Dead'", 20, yPos, 14, aliveHeaderColor);
            yPos += 18;
            DrawText("button to dispose", 20, yPos, 14, aliveHeaderColor);
            yPos += 18;
            DrawText("of it.", 20, yPos, 14, aliveHeaderColor);
        } else {
            Color statusHeader = manager->IsAlternativeColors()
                ? Color{200, 200, 200, 255}  // Light grey
                : Color{60, 60, 60, 255};    // Dark grey
            DrawText("PLANT STATUS:", 20, yPos, 14, statusHeader);
            yPos += 25;

            std::ostringstream healthStream;
            healthStream << "Health: " << selectedPlant->getHealthLevel() << "%";
            Color healthColor = selectedPlant->getHealthLevel() > 70 ? Color{120, 165, 120, 255} :
                                selectedPlant->getHealthLevel() > 40 ? Color{235, 186, 170, 255} : Color{200, 100, 100, 255};
            DrawText(healthStream.str().c_str(), 20, yPos, 14, healthColor);
            yPos += 22;

            std::ostringstream waterStream;
            waterStream << "Water: " << selectedPlant->getWaterLevel() << "%";
            DrawText(waterStream.str().c_str(), 20, yPos, 14, statusHeader);
            yPos += 22;

            std::ostringstream nutrientStream;
            nutrientStream << "Nutrients: " << selectedPlant->getNutrientLevel() << "%";
            Color nutrientColor = selectedPlant->getNutrientLevel() > 50 ? Color{120, 165, 120, 255} :
                                  selectedPlant->getNutrientLevel() > 25 ? Color{235, 186, 170, 255} : Color{200, 100, 100, 255};
            DrawText(nutrientStream.str().c_str(), 20, yPos, 14, nutrientColor);
            yPos += 22;

            std::ostringstream sunStream;
            sunStream << "Sunlight: " << selectedPlant->getSunlightExposure() << "%";
            DrawText(sunStream.str().c_str(), 20, yPos, 14, statusHeader);
            yPos += 30;
        }
    } else {
        Color instructHeader = manager->IsAlternativeColors()
            ? Color{255, 180, 160, 255}  // Light terracotta
            : Color{235, 186, 170, 255}; // Warm terracotta
        Color instructText = manager->IsAlternativeColors()
            ? Color{200, 200, 200, 255}  // Light grey
            : Color{85, 107, 95, 255};   // Dark forest green

        DrawText("INSTRUCTIONS:", 20, yPos, 16, instructHeader);
        yPos += 30;

        DrawText("1. Click a plant to select", 20, yPos, 13, instructText);
        yPos += 22;
        DrawText("2. Use care buttons", 20, yPos, 13, instructText);
        yPos += 22;
        DrawText("3. Monitor health status", 20, yPos, 13, instructText);
        yPos += 22;
        DrawText("4. Transfer when ready", 20, yPos, 13, instructText);
        yPos += 18;
        DrawText("   for sale", 20, yPos, 13, instructText);
        yPos += 22;
        DrawText("5. Remove dead plants", 20, yPos, 13, instructText);
    }
}

void StaffGreenhouseScreen::DrawMiddlePanel() {
    Color panelBg = manager->IsAlternativeColors()
        ? Color{40, 45, 45, 255}     // Dark panel
        : Color{206, 237, 223, 255}; // Soft mint
    DrawRectangle(leftPanelWidth, 0, middlePanelWidth, screenHeight, panelBg);

    Color borderColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint border
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawLine(leftPanelWidth + middlePanelWidth, 0, leftPanelWidth + middlePanelWidth, screenHeight, borderColor);

    const char* header = "STAFF GREENHOUSE - PLANT CARE";
    int headerSize = 22;
    int headerWidth = MeasureText(header, headerSize);
    Color headerColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawText(header,
             leftPanelWidth + (middlePanelWidth - headerWidth) / 2,
             20,
             headerSize,
             headerColor);

    DrawGrid();
}

void StaffGreenhouseScreen::DrawRightPanel() {
    Color panelBg = manager->IsAlternativeColors()
        ? Color{50, 45, 40, 255}     // Dark peachy panel
        : Color{255, 236, 214, 255}; // Peachy cream
    DrawRectangle(leftPanelWidth + middlePanelWidth, 0, rightPanelWidth, screenHeight, panelBg);

    const char* header = "CARE ACTIONS";
    int headerSize = 18;
    int headerWidth = MeasureText(header, headerSize);
    Color headerColor = manager->IsAlternativeColors()
        ? Color{255, 200, 150, 255}  // Light peachy
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawText(header,
             screenWidth - rightPanelWidth + (rightPanelWidth - headerWidth) / 2,
             20,
             headerSize,
             headerColor);

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

            Color cellColorIdle = manager->IsAlternativeColors()
                ? Color{50, 55, 50, 255}     // Dark cell
                : Color{245, 250, 247, 255}; // Light cell
            Color cellColorSelected = manager->IsAlternativeColors()
                ? Color{80, 90, 60, 255}     // Dark yellow-ish selected
                : Color{255, 247, 204, 255}; // Light yellow selected
            Color cellColorDeadSelected = manager->IsAlternativeColors()
                ? Color{90, 50, 50, 255}     // Dark red selected
                : Color{255, 220, 220, 255}; // Light red selected
            Color cellColorDead = manager->IsAlternativeColors()
                ? Color{70, 45, 45, 255}     // Dark red-ish
                : Color{245, 215, 215, 255}; // Light red-ish

            Color cellColor = cellColorIdle;
            if (row == selectedRow && col == selectedCol) {
                cellColor = isDead ? cellColorDeadSelected : cellColorSelected;
            } else if (isDead) {
                cellColor = cellColorDead;
            }

            DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, cellColor);

            Color borderColor = manager->IsAlternativeColors()
                ? (isDead ? Color{255, 140, 140, 255} : Color{100, 110, 100, 255})
                : (isDead ? Color{200, 100, 100, 255} : Color{200, 210, 205, 255});
            DrawRectangleLines(x + 1, y + 1, cellSize - 2, cellSize - 2, borderColor);

            if (plant != nullptr) {
                DrawPlantInCell(plant, row, col);
                if (isDead) {
                    const char* skullIcon = "X";
                    int iconWidth = MeasureText(skullIcon, 30);
                    Color skullColor = manager->IsAlternativeColors()
                        ? Color{255, 140, 140, 255}  // Light red
                        : Color{200, 100, 100, 255}; // Red
                    DrawText(skullIcon, x + (cellSize - iconWidth) / 2, y + cellSize / 2 - 15, 30, skullColor);
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
        Color deadTextColor = manager->IsAlternativeColors()
            ? Color{255, 140, 140, 255}  // Light red
            : Color{200, 100, 100, 255}; // Red
        Color aliveTextColor = manager->IsAlternativeColors()
            ? Color{180, 200, 190, 255}  // Light grey-green
            : Color{85, 107, 95, 255};   // Dark forest green
        Color textColor = isDead ? deadTextColor : aliveTextColor;
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

        Color healthBarBg = manager->IsAlternativeColors()
            ? Color{60, 65, 60, 255}     // Dark grey background
            : Color{200, 210, 205, 255}; // Light grey background
        DrawRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight, healthBarBg);
        DrawRectangle(healthBarX, healthBarY, (healthBarWidth * health) / 100, healthBarHeight, healthColor);
    }
}

void StaffGreenhouseScreen::DrawButtons() {
    bool hasSelection = selectedPlant != nullptr;
    bool isDead = IsPlantDead(selectedPlant);
    bool canCare = hasSelection && !isDead;

    Color borderColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint border
        : BLACK;                     // Black border
    Color disabledColor = manager->IsAlternativeColors()
        ? Color{60, 60, 65, 255}     // Dark disabled
        : Color{180, 180, 180, 255}; // Light disabled
    Color activeTextDark = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : BLACK;                     // Black text
    Color disabledText = manager->IsAlternativeColors()
        ? Color{100, 100, 100, 255}  // Dark grey
        : DARKGRAY;                  // Dark grey

    // Water: soft teal
    Color waterBase = manager->IsAlternativeColors()
        ? Color{60, 100, 110, 255}   // Dark teal
        : Color{120, 200, 220, 255}; // Soft teal
    Color waterHover = manager->IsAlternativeColors()
        ? Color{70, 120, 130, 255}   // Lighter teal
        : Color{100, 180, 200, 255}; // Darker teal
    Color waterColor = canCare ? (waterHovered ? waterHover : waterBase) : disabledColor;
    DrawRectangleRec(waterButton, waterColor);
    DrawRectangleLinesEx(waterButton, 2, borderColor);
    const char* waterText = "Water";
    int waterTextWidth = MeasureText(waterText, 16);
    DrawText(waterText, waterButton.x + (waterButton.width - waterTextWidth) / 2,
             waterButton.y + (waterButton.height - 16) / 2, 16, canCare ? activeTextDark : disabledText);

    // Fertilize: soft green
    Color fertBase = manager->IsAlternativeColors()
        ? Color{70, 100, 80, 255}    // Dark green
        : Color{150, 210, 160, 255}; // Soft green
    Color fertHover = manager->IsAlternativeColors()
        ? Color{85, 120, 95, 255}    // Lighter green
        : Color{130, 190, 140, 255}; // Darker green
    Color fertColor = canCare ? (fertilizeHovered ? fertHover : fertBase) : disabledColor;
    DrawRectangleRec(fertilizeButton, fertColor);
    DrawRectangleLinesEx(fertilizeButton, 2, borderColor);
    const char* fertText = "Fertilize";
    int fertTextWidth = MeasureText(fertText, 16);
    DrawText(fertText, fertilizeButton.x + (fertilizeButton.width - fertTextWidth) / 2,
             fertilizeButton.y + (fertilizeButton.height - 16) / 2, 16, activeTextDark);

    // Sunlight: light butter
    Color sunBase = manager->IsAlternativeColors()
        ? Color{120, 110, 50, 255}   // Dark yellow
        : Color{255, 235, 150, 255}; // Light butter
    Color sunHover = manager->IsAlternativeColors()
        ? Color{140, 130, 60, 255}   // Lighter yellow
        : Color{255, 225, 120, 255}; // Darker butter
    Color sunColor = canCare ? (sunlightHovered ? sunHover : sunBase) : disabledColor;
    DrawRectangleRec(adjustSunlightButton, sunColor);
    DrawRectangleLinesEx(adjustSunlightButton, 2, borderColor);
    const char* sunText = "Sunlight";
    int sunTextWidth = MeasureText(sunText, 16);
    DrawText(sunText, adjustSunlightButton.x + (adjustSunlightButton.width - sunTextWidth) / 2,
             adjustSunlightButton.y + (adjustSunlightButton.height - 16) / 2, 16, activeTextDark);

    // Full care: soft lavender
    Color fullBase = manager->IsAlternativeColors()
        ? Color{85, 70, 100, 255}    // Dark lavender
        : Color{190, 160, 210, 255}; // Soft lavender
    Color fullHover = manager->IsAlternativeColors()
        ? Color{100, 85, 115, 255}   // Lighter lavender
        : Color{175, 145, 195, 255}; // Darker lavender
    Color fullCareColor = canCare ? (fullCareHovered ? fullHover : fullBase) : disabledColor;
    DrawRectangleRec(performFullCareButton, fullCareColor);
    DrawRectangleLinesEx(performFullCareButton, 3, borderColor);
    const char* fullCareText = "FULL CARE";
    int fullCareTextWidth = MeasureText(fullCareText, 18);
    Color fullCareTextColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : WHITE;                     // White text
    DrawText(fullCareText, performFullCareButton.x + (performFullCareButton.width - fullCareTextWidth) / 2,
             performFullCareButton.y + (performFullCareButton.height - 18) / 2, 18, fullCareTextColor);

    // Transfer: soft sky blue
    bool canTransfer = canCare && selectedPlant->isReadyForSale();
    Color transBase = manager->IsAlternativeColors()
        ? Color{60, 80, 100, 255}    // Dark blue
        : Color{140, 180, 230, 255}; // Soft sky blue
    Color transHover = manager->IsAlternativeColors()
        ? Color{70, 95, 120, 255}    // Lighter blue
        : Color{120, 160, 210, 255}; // Darker blue
    Color transferColor = canTransfer ? (transferHovered ? transHover : transBase) : disabledColor;
    DrawRectangleRec(transferToSalesFloorButton, transferColor);
    DrawRectangleLinesEx(transferToSalesFloorButton, 3, borderColor);
    const char* transferText = "To Sales Floor";
    int transferTextWidth = MeasureText(transferText, 16);
    Color transferTextColor = manager->IsAlternativeColors()
        ? (canTransfer ? Color{240, 240, 240, 255} : disabledText)
        : (canTransfer ? WHITE : DARKGRAY);
    DrawText(transferText, transferToSalesFloorButton.x + (transferToSalesFloorButton.width - transferTextWidth) / 2,
             transferToSalesFloorButton.y + (transferToSalesFloorButton.height - 16) / 2, 16,
             transferTextColor);

    // Remove dead: soft red
    bool canRemove = hasSelection && isDead;
    Color remBase = manager->IsAlternativeColors()
        ? Color{100, 50, 50, 255}    // Dark red
        : Color{230, 120, 120, 255}; // Soft red
    Color remHover = manager->IsAlternativeColors()
        ? Color{120, 60, 60, 255}    // Lighter red
        : Color{210, 90, 90, 255};   // Darker red
    Color removeColor = canRemove ? (removeDeadHovered ? remHover : remBase) : disabledColor;
    DrawRectangleRec(removeDeadPlantButton, removeColor);
    DrawRectangleLinesEx(removeDeadPlantButton, 3, borderColor);
    const char* removeText = "Remove Dead";
    int removeTextWidth = MeasureText(removeText, 15);
    Color removeTextColor = manager->IsAlternativeColors()
        ? (canRemove ? Color{240, 240, 240, 255} : disabledText)
        : (canRemove ? WHITE : DARKGRAY);
    DrawText(removeText, removeDeadPlantButton.x + (removeDeadPlantButton.width - removeTextWidth) / 2,
             removeDeadPlantButton.y + (removeDeadPlantButton.height - 15) / 2, 15,
             removeTextColor);

    // Advance day
    Color advanceBase = manager->IsAlternativeColors()
        ? Color{120, 70, 20, 255}    // Dark orange
        : Color{255, 150, 0, 255};   // Orange
    Color advanceHover = manager->IsAlternativeColors()
        ? Color{140, 90, 30, 255}    // Lighter orange
        : Color{255, 200, 0, 255};   // Yellow-orange
    Color advanceDayColor = advanceDayHovered ? advanceHover : advanceBase;
    DrawRectangleRec(advanceDayButton, advanceDayColor);
    DrawRectangleLinesEx(advanceDayButton, 3, borderColor);
    const char* advanceDayText = "Advance Day";
    int advanceDayTextWidth = MeasureText(advanceDayText, 16);
    DrawText(advanceDayText, advanceDayButton.x + (advanceDayButton.width - advanceDayTextWidth) / 2,
             advanceDayButton.y + (advanceDayButton.height - 16) / 2, 16, activeTextDark);

    // Run Scheduler: SAME colors as Advance Day
    Color schedulerColor = schedulerHovered ? advanceHover : advanceBase;
    DrawRectangleRec(runSchedulerButton, schedulerColor);
    DrawRectangleLinesEx(runSchedulerButton, 2, borderColor);
    const char* schedulerText = "Run Scheduler";
    int schedulerTextWidth = MeasureText(schedulerText, 15);
    DrawText(schedulerText, runSchedulerButton.x + (runSchedulerButton.width - schedulerTextWidth) / 2,
             runSchedulerButton.y + (runSchedulerButton.height - 15) / 2, 15, activeTextDark);

    // Back: neutral grey
    Color backBase = manager->IsAlternativeColors()
        ? Color{50, 50, 50, 255}     // Dark grey
        : Color{100, 100, 100, 255}; // Medium grey
    Color backHoverColor = manager->IsAlternativeColors()
        ? Color{70, 70, 70, 255}     // Lighter grey
        : DARKGRAY;                  // Dark grey
    Color backColor = backHovered ? backHoverColor : backBase;
    DrawRectangleRec(backButton, backColor);
    DrawRectangleLinesEx(backButton, 2, borderColor);
    const char* backText = "Back";
    int backTextWidth = MeasureText(backText, 16);
    Color backTextColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : WHITE;                     // White text
    DrawText(backText, backButton.x + (backButton.width - backTextWidth) / 2,
             backButton.y + (backButton.height - 16) / 2, 16, backTextColor);

    // View Sales Floor button
    Color viewBase = manager->IsAlternativeColors()
        ? Color{60, 80, 100, 255}    // Dark blue
        : Color{140, 180, 230, 255}; // Soft sky blue
    Color viewHover = manager->IsAlternativeColors()
        ? Color{70, 95, 120, 255}    // Lighter blue
        : Color{120, 160, 210, 255}; // Darker blue
    Color viewColor = viewSalesFloorHovered ? viewHover : viewBase;
    DrawRectangleRec(viewSalesFloorButton, viewColor);
    DrawRectangleLinesEx(viewSalesFloorButton, 3, borderColor);
    const char* viewText = "View Sales Floor";
    int viewW = MeasureText(viewText, 16);
    Color viewTextColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : WHITE;                     // White text
    DrawText(viewText,
            viewSalesFloorButton.x + (viewSalesFloorButton.width - viewW)/2,
            viewSalesFloorButton.y + (viewSalesFloorButton.height - 16)/2,
            16, viewTextColor);
}

void StaffGreenhouseScreen::DrawSchedulerInfo() {
    int yPos = screenHeight - 250;

    Color lineColor = manager->IsAlternativeColors()
        ? Color{80, 80, 85, 255}     // Dark line
        : Color{200, 210, 205, 255}; // Light line
    DrawLine(screenWidth - rightPanelWidth + 20, yPos, screenWidth - 20, yPos, lineColor);
    yPos += 10;

    Color headerColor = manager->IsAlternativeColors()
        ? Color{200, 200, 200, 255}  // Light grey
        : Color{60, 60, 60, 255};    // Dark grey
    DrawText("SCHEDULER:", screenWidth - rightPanelWidth + 20, yPos, 14, headerColor);
    yPos += 22;

    CareScheduler* scheduler = manager->GetCareScheduler();
    if (scheduler != nullptr && !scheduler->empty()) {
        DrawText("Tasks queued", screenWidth - rightPanelWidth + 20, yPos, 12, headerColor);
    } else {
        Color noTasksColor = manager->IsAlternativeColors()
            ? Color{140, 160, 150, 255}  // Medium grey-green
            : Color{120, 140, 125, 255}; // Medium sage
        DrawText("No tasks", screenWidth - rightPanelWidth + 20, yPos, 12, noTasksColor);
    }
}

void StaffGreenhouseScreen::DrawDayCounter() {
    int days = manager->GetDaysCounter();
    std::ostringstream dayStream;
    dayStream << "Day: " << days;
    Color dayColor = manager->IsAlternativeColors()
        ? Color{255, 180, 160, 255}  // Light terracotta
        : Color{235, 186, 170, 255}; // Warm terracotta
    DrawText(dayStream.str().c_str(), 20, 85, 24, dayColor);
}

void StaffGreenhouseScreen::DrawSchedulerOverlay() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});

    int overlayWidth  = 650;
    int overlayHeight = 500;
    int overlayX      = (screenWidth - overlayWidth) / 2;
    int overlayY      = (screenHeight - overlayHeight) / 2;

    Color overlayBg = manager->IsAlternativeColors()
        ? Color{45, 45, 50, 255}     // Dark lavender
        : Color{230, 224, 237, 255}; // Soft lavender
    DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, overlayBg);

    Color overlayBorder = manager->IsAlternativeColors()
        ? Color{255, 180, 160, 255}  // Light terracotta
        : Color{235, 186, 170, 255}; // Terracotta border
    DrawRectangleLinesEx(Rectangle{(float)overlayX, (float)overlayY, (float)overlayWidth, (float)overlayHeight},
                         3, overlayBorder);

    const char* title = "CARE SCHEDULER";
    int titleWidth = MeasureText(title, 24);
    Color titleColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{50, 70, 60, 255};    // darker green
    DrawText(title, overlayX + (overlayWidth - titleWidth) / 2, overlayY + 20, 24, titleColor);

    int yPos = overlayY + 60;

    Color textMain = manager->IsAlternativeColors()
        ? Color{220, 220, 220, 255}  // Light grey
        : Color{40, 40, 40, 255};    // Dark grey
    Color textSub = manager->IsAlternativeColors()
        ? Color{180, 180, 180, 255}  // Medium grey
        : Color{70, 70, 70, 255};    // Dark grey

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
                Color moreColor = manager->IsAlternativeColors()
                    ? Color{120, 120, 120, 255}  // Medium grey
                    : Color{100, 100, 100, 255}; // Dark grey
                DrawText(remaining.str().c_str(), overlayX + 30, yPos, 14, moreColor);
                yPos += 25;
            }

            std::ostringstream totalText;
            totalText << "Total: " << displayedQueueSize << " tasks queued";
            DrawText(totalText.str().c_str(), overlayX + 30, yPos, 16, textMain);
            yPos += 15;
        }

        yPos += 20;
        Color lineColor = manager->IsAlternativeColors()
            ? Color{80, 80, 85, 255}  // Dark grey line
            : DARKGRAY;               // Dark grey
        DrawLine(overlayX + 20, yPos, overlayX + overlayWidth - 20, yPos, lineColor);
        yPos += 25;

        DrawText("Choose an action:", overlayX + 20, yPos, 16, textMain);
        yPos += 35;
    } else {
        DrawText("No tasks in queue!", overlayX + 30, yPos, 18, textMain);
        yPos += 40;
        DrawText("All plants are well cared for.", overlayX + 30, yPos, 14, textSub);
    }

    Color buttonBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : BLACK;                     // Black
    Color buttonText = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : BLACK;                     // Black text

    // Overlay buttons
    Color runAllBase = manager->IsAlternativeColors()
        ? Color{70, 100, 80, 255}    // Dark green
        : Color{160, 220, 160, 255}; // Light green
    Color runAllHover = manager->IsAlternativeColors()
        ? Color{85, 120, 95, 255}    // Lighter green
        : Color{130, 200, 130, 255}; // Darker green
    Color runAllColor = runAllHovered ? runAllHover : runAllBase;
    DrawRectangleRec(runAllButton, runAllColor);
    DrawRectangleLinesEx(runAllButton, 2, buttonBorder);
    const char* runAllText = "Run All & Advance Day";
    int runAllTextWidth = MeasureText(runAllText, 16);
    DrawText(runAllText, runAllButton.x + (runAllButton.width - runAllTextWidth) / 2,
             runAllButton.y + (runAllButton.height - 16) / 2, 16, buttonText);

    Color runOneBase = manager->IsAlternativeColors()
        ? Color{120, 90, 50, 255}    // Dark orange
        : Color{255, 200, 120, 255}; // Light orange
    Color runOneHover = manager->IsAlternativeColors()
        ? Color{140, 110, 60, 255}   // Lighter orange
        : Color{255, 180, 90, 255};  // Darker orange
    Color runOneColor = runOneHovered ? runOneHover : runOneBase;
    DrawRectangleRec(runOneButton, runOneColor);
    DrawRectangleLinesEx(runOneButton, 2, buttonBorder);
    const char* runOneText = "Run One Task";
    int runOneTextWidth = MeasureText(runOneText, 16);
    DrawText(runOneText, runOneButton.x + (runOneButton.width - runOneTextWidth) / 2,
             runOneButton.y + (runOneButton.height - 16) / 2, 16, buttonText);

    Color closeBase = manager->IsAlternativeColors()
        ? Color{50, 50, 50, 255}     // Dark grey
        : GRAY;                      // Grey
    Color closeHover = manager->IsAlternativeColors()
        ? Color{70, 70, 70, 255}     // Lighter grey
        : DARKGRAY;                  // Dark grey
    Color closeColor = closeOverlayHovered ? closeHover : closeBase;
    DrawRectangleRec(closeOverlayButton, closeColor);
    DrawRectangleLinesEx(closeOverlayButton, 2, buttonBorder);
    const char* closeText = schedulerTasksExecuted ? "Close & Advance Day" : "Close";
    int closeTextWidth = MeasureText(closeText, 14);
    Color closeTextColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : WHITE;                     // White text
    DrawText(closeText, closeOverlayButton.x + (closeOverlayButton.width - closeTextWidth) / 2,
             closeOverlayButton.y + (closeOverlayButton.height - 14) / 2, 14, closeTextColor);

    Color hintColor = manager->IsAlternativeColors()
        ? Color{140, 140, 140, 255}  // Medium grey
        : DARKGRAY;                  // Dark grey
    DrawText(schedulerTasksExecuted ? "Press ESC to close & advance day" : "Press ESC to close",
             overlayX + 20, overlayY + overlayHeight - 20, 12, hintColor);
}
