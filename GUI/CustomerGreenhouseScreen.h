#ifndef CUSTOMER_GREENHOUSE_SCREEN_H
#define CUSTOMER_GREENHOUSE_SCREEN_H

#include "raylib.h"
#include <string>

class ScreenManager;
class Plant;

class CustomerGreenhouseScreen {
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
    Rectangle backToSalesFloorButton;
    Rectangle viewCartButton;
    
    // Button hover states
    bool backButtonHovered;
    bool viewCartHovered;
    
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
    
    void HandlePlantSelection(int row, int col);
    
public:
    CustomerGreenhouseScreen(ScreenManager* mgr);
    ~CustomerGreenhouseScreen();
    
    void Update();
    void Draw();
};

#endif