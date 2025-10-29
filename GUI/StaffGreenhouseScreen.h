#ifndef STAFF_GREENHOUSE_SCREEN_H
#define STAFF_GREENHOUSE_SCREEN_H

#include "raylib.h"
#include <string>
#include <vector>

class ScreenManager;
class Plant;

class StaffGreenhouseScreen {
private:
    ScreenManager* manager;
    
    // Layout dimensions
    int leftPanelWidth;
    int middlePanelWidth;
    int rightPanelWidth;
    int screenWidth;
    int screenHeight;
    
    // Grid dimensions
    int gridRows;
    int gridCols;
    int cellSize;
    int gridStartX;
    int gridStartY;
    
    // Selection state
    int selectedRow;
    int selectedCol;
    Plant* selectedPlant;
    
    // UI Buttons
    Rectangle waterButton;
    Rectangle fertilizeButton;
    Rectangle adjustSunlightButton;
    Rectangle performFullCareButton;
    Rectangle transferToSalesFloorButton;
    Rectangle runSchedulerButton;
    Rectangle advanceDayButton;  // NEW
    Rectangle backButton;
    
    // Button hover states
    bool waterHovered;
    bool fertilizeHovered;
    bool sunlightHovered;
    bool fullCareHovered;
    bool transferHovered;
    bool schedulerHovered;
    bool advanceDayHovered;  // NEW
    bool backHovered;
    
    // Care scheduler info
    int scheduledTasksCount;
    
    void InitializeLayout();
    void InitializeButtons();
    void UpdateGrid();
    void UpdateButtons();
    
    void DrawLeftPanel();
    void DrawMiddlePanel();
    void DrawRightPanel();
    void DrawGrid();
    void DrawPlantInCell(Plant* plant, int row, int col);
    void DrawButtons();
    void DrawSchedulerInfo();
    void DrawDayCounter();  // NEW
    
    void HandlePlantSelection(int row, int col);
    void HandleWaterPlant();
    void HandleFertilizePlant();
    void HandleAdjustSunlight();
    void HandleFullCare();
    void HandleTransferToSalesFloor();
    void HandleRunScheduler();
    void HandleAdvanceDay();  // NEW
    
public:
    StaffGreenhouseScreen(ScreenManager* mgr);
    ~StaffGreenhouseScreen();
    
    void Update();
    void Draw();
};

#endif