#include "StaffSalesFloorScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <sstream>

// backend
#include "../include/SalesFloor.h"
#include "../include/Greenhouse.h"     // only for textures util if needed
#include "../include/Plant.h"

StaffSalesFloorScreen::StaffSalesFloorScreen(ScreenManager* mgr)
    : manager(mgr) {

    // mirror the customer sales grid size (usually 5x5); fall back to 5x5
    SalesFloor* sf = manager->GetSalesFloor();
    gridRows = sf ? sf->getRows() : 5;
    gridCols = sf ? sf->getColumns() : 5;

    InitializeLayout();
    InitializeButtons();
}

StaffSalesFloorScreen::~StaffSalesFloorScreen() {}

void StaffSalesFloorScreen::InitializeLayout() {
    screenWidth  = GetScreenWidth();
    screenHeight = GetScreenHeight();

    leftPanelWidth   = screenWidth / 5;
    middlePanelWidth = screenWidth * 3 / 5;
    rightPanelWidth  = screenWidth / 5;

    int availW = middlePanelWidth - 40;          // padding
    int availH = screenHeight - 120;             // header + padding
    int byW = availW / gridCols;
    int byH = availH / gridRows;
    cellSize = (byW < byH ? byW : byH);

    int gridW = cellSize * gridCols;
    int gridH = cellSize * gridRows;

    gridStartX = leftPanelWidth + (middlePanelWidth - gridW) / 2;
    gridStartY = 80 + (screenHeight - 100 - gridH) / 2;
}

void StaffSalesFloorScreen::InitializeButtons() {
    const int bw = 220, bh = 44;
    backButton = Rectangle{
        (float)(screenWidth - rightPanelWidth + (rightPanelWidth - bw)/2),
        (float)(screenHeight - 70),
        (float)bw, (float)bh
    };
}

void StaffSalesFloorScreen::Update() {
    UpdateGrid();
    UpdateButtons();
}

void StaffSalesFloorScreen::UpdateGrid() {
    Vector2 m = GetMousePosition();

    if (m.x >= gridStartX && m.x < gridStartX + cellSize*gridCols &&
        m.y >= gridStartY && m.y < gridStartY + cellSize*gridRows) {

        int col = (int)((m.x - gridStartX) / cellSize);
        int row = (int)((m.y - gridStartY) / cellSize);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            SalesFloor* sf = manager->GetSalesFloor();
            if (!sf) return;
            Plant* p = sf->getPlantAt(row, col);

            selectedRow = p ? row : -1;
            selectedCol = p ? col : -1;
            selectedPlant = p;

            if (p) {
                std::cout << "[StaffSalesFloor] Selected " << p->getName()
                          << " (ID: " << p->getID() << ") at (" << row << "," << col << ")\n";
            }
        }
    }
}

void StaffSalesFloorScreen::UpdateButtons() {
    Vector2 m = GetMousePosition();
    backHovered = CheckCollisionPointRec(m, backButton);

    if (backHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        manager->SwitchScreen(GameScreen::STAFF_GREENHOUSE);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        manager->SwitchScreen(GameScreen::STAFF_GREENHOUSE);
    }
}

void StaffSalesFloorScreen::Draw() {
    // soft sage bg overall
    ClearBackground(Color{216, 228, 220, 255});

    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();
}

void StaffSalesFloorScreen::DrawLeftPanel() {
    // Left meta/info panel (no bullet symbols)
    DrawRectangle(0, 0, leftPanelWidth, screenHeight, Color{230, 224, 237, 255}); // soft lavender
    DrawLine(leftPanelWidth, 0, leftPanelWidth, screenHeight, Color{85, 107, 95, 255});

    int y = 18;
    DrawText("STAFF MODE", 18, y, 22, Color{235, 186, 170, 255});
    y += 36;

    DrawText("Sales Floor (view only)", 18, y, 16, Color{120, 140, 125, 255});
    y += 28;

    DrawDayCounter();
    y += 50;

    DrawLine(18, y, leftPanelWidth-18, y, Color{85, 107, 95, 255});
    y += 14;

    if (selectedPlant) {
        const bool dead = IsPlantDead(selectedPlant);

        DrawText("SELECTED PLANT", 18, y, 18, dead ? Color{200,100,100,255} : Color{235,186,170,255});
        y += 28;

        std::string nm = selectedPlant->getName();
        DrawText(nm.c_str(), 18, y, 18, Color{85,107,95,255}); y += 26;

        std::string id = std::string("ID: ") + selectedPlant->getID();
        DrawText(id.c_str(), 18, y, 14, Color{120,140,125,255}); y += 22;

        std::string st = std::string("State: ") + selectedPlant->getState()->getStateName();
        DrawText(st.c_str(), 18, y, 14, dead ? Color{200,100,100,255} : Color{120,140,125,255}); y += 22;

        // Show some basic stats useful for staff
        std::ostringstream age; age << "Age: " << selectedPlant->getAge() << " days";
        DrawText(age.str().c_str(), 18, y, 14, Color{120,140,125,255}); y += 20;

        // If your Plant exposes price, you can include it here:
        // std::ostringstream price; price << "Price: R" << std::fixed << std::setprecision(2)
        //                                   << selectedPlant->getPrice();
        // DrawText(price.str().c_str(), 18, y, 14, Color{120,140,125,255}); y += 20;

        DrawLine(18, y, leftPanelWidth-18, y, Color{200,210,205,255}); y += 14;

        if (!dead) {
            std::ostringstream h; h << "Health: " << selectedPlant->getHealthLevel() << "%";
            Color hc = selectedPlant->getHealthLevel() > 70 ? Color{120,165,120,255}
                        : selectedPlant->getHealthLevel() > 40 ? Color{235,186,170,255}
                        : Color{200,100,100,255};
            DrawText(h.str().c_str(), 18, y, 14, hc); y += 20;

            std::ostringstream w; w << "Water: " << selectedPlant->getWaterLevel() << "%";
            DrawText(w.str().c_str(), 18, y, 14, Color{60,60,60,255}); y += 20;

            std::ostringstream n; n << "Nutrients: " << selectedPlant->getNutrientLevel() << "%";
            DrawText(n.str().c_str(), 18, y, 14, Color{60,60,60,255}); y += 20;

            std::ostringstream s; s << "Sunlight: " << selectedPlant->getSunlightExposure() << "%";
            DrawText(s.str().c_str(), 18, y, 14, Color{60,60,60,255}); y += 20;
        } else {
            DrawText("This plant is dead.", 18, y, 14, Color{200,100,100,255}); y += 20;
        }
    } else {
        DrawText("Click a plant to view details.", 18, y, 14, Color{85,107,95,255});
    }
}

void StaffSalesFloorScreen::DrawMiddlePanel() {
    DrawRectangle(leftPanelWidth, 0, middlePanelWidth, screenHeight, Color{206, 237, 223, 255}); // mint
    DrawLine(leftPanelWidth + middlePanelWidth, 0, leftPanelWidth + middlePanelWidth, screenHeight,
             Color{85, 107, 95, 255});

    const char* header = "STAFF SALES FLOOR VIEW ONLY";
    int size = 22;
    int w = MeasureText(header, size);
    DrawText(header, leftPanelWidth + (middlePanelWidth - w)/2, 16, size, Color{85,107,95,255});

    DrawGrid();
}

void StaffSalesFloorScreen::DrawRightPanel() {
    // minimalist right panel (only back button)
    DrawRectangle(leftPanelWidth + middlePanelWidth, 0, rightPanelWidth, screenHeight, Color{255, 236, 214, 255}); // peach
    const char* h = "NAVIGATION";
    int size = 18; int w = MeasureText(h, size);
    DrawText(h, screenWidth - rightPanelWidth + (rightPanelWidth - w)/2, 16, size, Color{85,107,95,255});

    Color btn = backHovered ? Color{50,130,80,255} : Color{60,150,100,255};
    DrawRectangleRec(backButton, btn);
    DrawRectangleLinesEx(backButton, 3, WHITE);
    const char* t = "Back to Greenhouse";
    int tw = MeasureText(t, 18);
    DrawText(t, backButton.x + (backButton.width - tw)/2, backButton.y + 12, 18, WHITE);
}

void StaffSalesFloorScreen::DrawGrid() {
    SalesFloor* sf = manager->GetSalesFloor();
    if (!sf) return;

    for (int r = 0; r < gridRows; ++r) {
        for (int c = 0; c < gridCols; ++c) {
            int x = gridStartX + c*cellSize;
            int y = gridStartY + r*cellSize;

            Plant* p = sf->getPlantAt(r, c);
            bool isSel = (r == selectedRow && c == selectedCol);
            bool dead  = IsPlantDead(p);

            Color cell = Color{245,250,247,255};
            if (isSel) cell = dead ? Color{255,220,220,255} : Color{255,247,204,255};
            DrawRectangle(x+1, y+1, cellSize-2, cellSize-2, cell);

            DrawRectangleLines(x+1, y+1, cellSize-2, cellSize-2,
                               dead ? Color{200,100,100,255} : Color{200,210,205,255});

            if (p) DrawPlantInCell(p, r, c);
        }
    }
}

void StaffSalesFloorScreen::DrawPlantInCell(Plant* plant, int row, int col) {
    int x = gridStartX + col*cellSize;
    int y = gridStartY + row*cellSize;

    bool dead = IsPlantDead(plant);
    auto tex = manager->GetPlantTexture(plant->getName());

    if (tex.id != 0) {
        int maxSize = cellSize - 22;
        float scale = (float)maxSize / tex.height;
        int sw = (int)(tex.width * scale);
        int sh = (int)(tex.height * scale);
        int tx = x + (cellSize - sw)/2;
        int ty = y + (cellSize - sh)/2 - 4;
        DrawTextureEx(tex, Vector2{(float)tx, (float)ty}, 0.0f, scale, dead ? Color{110,110,110,255} : WHITE);
    } else {
        const char* name = plant->getName().c_str();
        int w = MeasureText(name, 10);
        DrawText(name, x + (cellSize - w)/2, y + cellSize/2 - 8, 10, dead ? Color{200,100,100,255} : Color{85,107,95,255});
    }

    // Tiny caption
    const char* cap = plant->getName().c_str();
    int cw = MeasureText(cap, 10);
    DrawText(cap, x + (cellSize - cw)/2, y + cellSize - 14, 10, Color{120,140,125,255});
}

void StaffSalesFloorScreen::DrawDayCounter() {
    int d = manager->GetDaysCounter();
    std::ostringstream ss; ss << "Day: " << d;
    DrawText(ss.str().c_str(), 18, 80, 22, Color{235,186,170,255});
}

bool StaffSalesFloorScreen::IsPlantDead(Plant* plant) const {
    if (!plant || !plant->getState()) return false;
    return plant->getState()->getStateName() == "Dead";
}
