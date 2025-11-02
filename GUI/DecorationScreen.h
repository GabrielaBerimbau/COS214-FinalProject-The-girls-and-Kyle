#ifndef DECORATION_SCREEN_H
#define DECORATION_SCREEN_H

#include "raylib.h"
#include <string>

class ScreenManager;
class Plant;

class DecorationScreen {
private:
    ScreenManager* manager;
    
    // Layout dimensions
    int screenWidth;
    int screenHeight;
    
    // Current state
    int cartIndex;
    Plant* currentPlant;
    std::string selectedPotColor;
    bool hasRibbon;
    bool hasPot;
    
    // Original state (for reverting)
    std::string originalPotColor;
    bool originalHasRibbon;
    bool originalHasPot;
    
    // UI Buttons
    Rectangle potButtons[10];
    Rectangle ribbonToggleButton;
    Rectangle removeRibbonButton;
    Rectangle removePotButton;
    Rectangle confirmButton;
    Rectangle backButton;
    
    // Button hover states
    bool potHovered[10];
    bool ribbonToggleHovered;
    bool removeRibbonHovered;
    bool removePotHovered;
    bool confirmHovered;
    bool backHovered;
    
    // Pot color names
    std::string potColors[10] = {
        "blue", "red", "terracotta", "green", "yellow",
        "pink", "purple", "orange", "black", "white"
    };
    
    void InitializeLayout();
    void InitializeButtons();
    void UpdateButtons();
    
    void DrawPlantPreview();
    void DrawPotSelection();
    void DrawRibbonOption();
    void DrawPriceInfo();
    void DrawButtons();
    
    void HandlePotSelection(int potIndex);
    void HandleRibbonToggle();
    void HandleRemoveRibbon();
    void HandleRemovePot();
    void HandleConfirm();
    void HandleBack();
    
    void LoadCurrentDecorations();
    void ApplyTemporaryDecorations();
    void RevertToOriginal();
    
    double CalculateCurrentPrice();
    double CalculateOriginalPrice();
    
public:
    DecorationScreen(ScreenManager* mgr);
    ~DecorationScreen();
    
    void Update();
    void Draw();
};

#endif