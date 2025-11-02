#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "raylib.h"

class ScreenManager;

class StartScreen {
private:
    ScreenManager* manager;
    
    // UI element positions and sizes
    Rectangle customerButton;
    Rectangle staffButton;
    
    // Colors
    Color customerButtonColor;
    Color staffButtonColor;
    
    // Hover states
    bool customerButtonHovered;
    bool staffButtonHovered;
    
    void InitializeUI();
    void UpdateButtons();
    void DrawBackground();
    void DrawTitle();
    void DrawButtons();
    
public:
    StartScreen(ScreenManager* mgr);
    ~StartScreen();
    
    void Update();
    void Draw();
};

#endif // START_SCREEN_H