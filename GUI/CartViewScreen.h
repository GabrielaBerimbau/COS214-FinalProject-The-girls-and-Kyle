#ifndef CART_VIEW_SCREEN_H
#define CART_VIEW_SCREEN_H

#include "raylib.h"
#include <string>

class ScreenManager;
class Plant;

class CartViewScreen {
private:
    ScreenManager* manager;
    
    // Layout dimensions
    int screenWidth;
    int screenHeight;
    
    // Current cart index being displayed
    int currentIndex;
    
    // UI Buttons
    Rectangle leftArrowButton;
    Rectangle rightArrowButton;
    Rectangle decorateButton;
    Rectangle backToSalesFloorButton;

    // Button hover states
    bool leftArrowHovered;
    bool rightArrowHovered;
    bool decorateHovered;
    bool backHovered;
    
    void InitializeLayout();
    void InitializeButtons();
    void UpdateButtons();
    
    void DrawPlantDisplay();
    void DrawPlantInfo();
    void DrawButtons();
    void DrawNavigationArrows();
    
    void HandleLeftArrow();
    void HandleRightArrow();
    void HandleDecorate();
    void HandleBack();
    
public:
    CartViewScreen(ScreenManager* mgr);
    ~CartViewScreen();
    
    void Update();
    void Draw();
};

#endif