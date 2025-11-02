#ifndef SALES_FLOOR_SCREEN_H
#define SALES_FLOOR_SCREEN_H

#include "raylib.h"
#include <string>

class ScreenManager;
class Plant;

class SalesFloorScreen {
private:
    ScreenManager* manager;
    
    int leftPanelWidth;
    int middlePanelWidth;
    int rightPanelWidth;
    int screenWidth;
    int screenHeight;
    
    int gridRows;
    int gridCols;
    int cellSize;
    int gridStartX;
    int gridStartY;
    
    int selectedRow;
    int selectedCol;
    Plant* selectedPlant;
    
    Rectangle addToCartButton;
    Rectangle viewGreenhouseButton;
    Rectangle viewCartButton;
    Rectangle createOrderButton;
    Rectangle makeRequestButton;
    Rectangle backToStartButton;

    bool addToCartHovered;
    bool viewGreenhouseHovered;
    bool viewCartHovered;
    bool createOrderHovered;
    bool makeRequestHovered;
    bool backToStartHovered;
    
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
    void HandleCreateOrder();
    void HandleMakeRequest();
    void HandleBackToStart();
    
public:
    SalesFloorScreen(ScreenManager* mgr);
    ~SalesFloorScreen();
    
    void Update();
    void Draw();
};

#endif