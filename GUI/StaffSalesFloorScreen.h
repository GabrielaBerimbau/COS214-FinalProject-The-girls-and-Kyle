#pragma once
#include "raylib.h"
#include <string>
#include <vector>

class ScreenManager;
class Plant;

class StaffSalesFloorScreen {
public:
    explicit StaffSalesFloorScreen(ScreenManager* mgr);
    ~StaffSalesFloorScreen();

    void Update();
    void Draw();

private:
    // wiring
    ScreenManager* manager;

    // layout
    int screenWidth{}, screenHeight{};
    int leftPanelWidth{}, middlePanelWidth{}, rightPanelWidth{};
    int gridRows{}, gridCols{};
    int cellSize{}, gridStartX{}, gridStartY{};

    // selection
    int selectedRow{-1}, selectedCol{-1};
    Plant* selectedPlant{nullptr};

    // ui
    Rectangle backButton{};
    bool backHovered{false};

    // init
    void InitializeLayout();
    void InitializeButtons();

    // update
    void UpdateGrid();
    void UpdateButtons();

    // draw
    void DrawLeftPanel();
    void DrawMiddlePanel();
    void DrawRightPanel();
    void DrawGrid();
    void DrawPlantInCell(Plant* plant, int row, int col);
    void DrawDayCounter();

    // helpers
    bool IsPlantDead(Plant* plant) const;
};
