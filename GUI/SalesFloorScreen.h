#ifndef SALES_FLOOR_SCREEN_H
#define SALES_FLOOR_SCREEN_H

#include "raylib.h"
#include <string>

class ScreenManager;
class Plant;

class SalesFloorScreen {
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
    Rectangle addToCartButton;
    Rectangle viewGreenhouseButton;
    Rectangle viewCartButton;
    Rectangle makeRequestButton;
    
    // Button hover states
    bool addToCartHovered;
    bool viewGreenhouseHovered;
    bool viewCartHovered;
    bool makeRequestHovered;
    
    // Request overlay state
    bool requestOverlayActive;
    char requestText[256];
    int requestTextLength;
    std::string responseText;
    
    void InitializeLayout();
    void InitializeButtons();
    void UpdateGrid();
    void UpdateButtons();
    void UpdateRequestOverlay();
    
    void DrawLeftPanel();
    void DrawMiddlePanel();
    void DrawRightPanel();
    void DrawGrid();
    void DrawPlantInCell(Plant* plant, int row, int col);
    void DrawButtons();
    void DrawRequestOverlay();
    
    void HandlePlantSelection(int row, int col);
    void HandleAddToCart();
    void HandleMakeRequest();
    
public:
    SalesFloorScreen(ScreenManager* mgr);
    ~SalesFloorScreen();
    
    void Update();
    void Draw();
};

#endif // SALES_FLOOR_SCREEN_H