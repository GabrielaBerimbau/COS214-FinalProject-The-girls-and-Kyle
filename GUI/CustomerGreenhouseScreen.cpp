#include "CustomerGreenhouseScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

// Backend includes
#include "../include/Customer.h"
#include "../include/Greenhouse.h"
#include "../include/Plant.h"

CustomerGreenhouseScreen::CustomerGreenhouseScreen(ScreenManager* mgr)
    : manager(mgr),
      selectedRow(-1),
      selectedCol(-1),
      selectedPlant(nullptr),
      backButtonHovered(false),
      viewCartHovered(false) {
    
    gridRows = 6;  // Greenhouse is 6x8
    gridCols = 8;
    
    InitializeLayout();
    InitializeButtons();
}

CustomerGreenhouseScreen::~CustomerGreenhouseScreen() {
    // Nothing to clean up
}

void CustomerGreenhouseScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    // 3-column layout: 25% | 50% | 25%
    leftPanelWidth = screenWidth / 5;
    middlePanelWidth = screenWidth * 3 / 5;
    rightPanelWidth = screenWidth / 5;
    
    // Calculate grid cell size (fits in middle panel with padding)
    int availableWidth = middlePanelWidth - 20;
    int availableHeight = screenHeight - 80;
    
    int cellSizeByWidth = availableWidth / gridCols;
    int cellSizeByHeight = availableHeight / gridRows;
    
    cellSize = (cellSizeByWidth < cellSizeByHeight) ? cellSizeByWidth : cellSizeByHeight;
    
    // Center the grid in the middle panel
    int gridTotalWidth = cellSize * gridCols;
    int gridTotalHeight = cellSize * gridRows;
    
    gridStartX = leftPanelWidth + (middlePanelWidth - gridTotalWidth) / 2;
    gridStartY = 60 + (screenHeight - 60 - gridTotalHeight) / 2;
}

void CustomerGreenhouseScreen::InitializeButtons() {
    int buttonWidth = rightPanelWidth - 40;
    int buttonHeight = 45;
    int buttonSpacing = 15;
    int buttonX = screenWidth - rightPanelWidth + 20;
    int startY = screenHeight - 150;
    
    backToSalesFloorButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    viewCartButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + buttonHeight + buttonSpacing),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
}

void CustomerGreenhouseScreen::Update() {
    UpdateGrid();
    UpdateButtons();
}

void CustomerGreenhouseScreen::UpdateGrid() {
    Vector2 mousePos = GetMousePosition();
    
    // Check if mouse is over grid
    if (mousePos.x >= gridStartX && mousePos.x < gridStartX + (cellSize * gridCols) &&
        mousePos.y >= gridStartY && mousePos.y < gridStartY + (cellSize * gridRows)) {
        
        int col = static_cast<int>((mousePos.x - gridStartX) / cellSize);
        int row = static_cast<int>((mousePos.y - gridStartY) / cellSize);
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            HandlePlantSelection(row, col);
        }
    }
}

void CustomerGreenhouseScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();
    
    // Update hover states
    backButtonHovered = CheckCollisionPointRec(mousePos, backToSalesFloorButton);
    viewCartHovered = CheckCollisionPointRec(mousePos, viewCartButton);
    
    // Handle button clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (backButtonHovered) {
            manager->SwitchScreen(GameScreen::SALES_FLOOR);
        } else if (viewCartHovered) {
            Customer* customer = manager->GetCustomer();
            if (customer != nullptr && customer->getCartSize() > 0) {
                manager->SwitchScreen(GameScreen::CART_VIEW);
            }
        }
    }
}

void CustomerGreenhouseScreen::HandlePlantSelection(int row, int col) {
    Greenhouse* greenhouse = manager->GetGreenhouse();
    if (greenhouse == nullptr) return;
    
    Plant* plant = greenhouse->getPlantAt(row, col);
    
    if (plant != nullptr) {
        selectedRow = row;
        selectedCol = col;
        selectedPlant = plant;
        std::cout << "[CustomerGreenhouseScreen] Selected plant: " << plant->getName() 
                  << " at (" << row << "," << col << ")" << std::endl;
    } else {
        selectedRow = -1;
        selectedCol = -1;
        selectedPlant = nullptr;
    }
}

void CustomerGreenhouseScreen::Draw() {
    Color bgColor = manager->IsAlternativeColors()
        ? Color{30, 30, 35, 255}     // Dark mode
        : Color{216, 228, 220, 255}; // Soft sage green
    ClearBackground(bgColor);

    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();
}

void CustomerGreenhouseScreen::DrawLeftPanel() {
    Color panelBg = manager->IsAlternativeColors()
        ? Color{40, 38, 45, 255}     // Dark lavender
        : Color{230, 224, 237, 255}; // Soft lavender
    Color lineColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    Color headerColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    Color textColor = manager->IsAlternativeColors()
        ? Color{200, 200, 200, 255}  // Light grey
        : Color{85, 107, 95, 255};   // Dark forest green
    Color budgetColor = manager->IsAlternativeColors()
        ? Color{150, 220, 150, 255}  // Light green
        : Color{120, 165, 120, 255}; // Soft green

    DrawRectangle(0, 0, leftPanelWidth, screenHeight, panelBg);
    DrawLine(leftPanelWidth, 0, leftPanelWidth, screenHeight, lineColor);

    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;

    int yPos = 20;

    DrawText("CUSTOMER INFO", 20, yPos, 20, headerColor);
    yPos += 35;

    std::string nameText = "Name: " + customer->getName();
    DrawText(nameText.c_str(), 20, yPos, 16, textColor);
    yPos += 25;

    std::ostringstream budgetStream;
    budgetStream << "Budget: R" << std::fixed << std::setprecision(2) << customer->getBudget();
    DrawText(budgetStream.str().c_str(), 20, yPos, 18, budgetColor);
    yPos += 45;

    DrawLine(20, yPos, leftPanelWidth - 20, yPos, lineColor);
    yPos += 15;

    Color accentColor = manager->IsAlternativeColors()
        ? Color{255, 150, 130, 255}  // Light coral
        : Color{235, 186, 170, 255}; // Warm terracotta
    Color detailColor = manager->IsAlternativeColors()
        ? Color{180, 200, 190, 255}  // Light grey-green
        : Color{120, 140, 125, 255}; // Medium sage
    Color separatorColor = manager->IsAlternativeColors()
        ? Color{80, 90, 85, 255}     // Dark grey-green
        : Color{200, 210, 205, 255}; // Light sage

    if (selectedPlant != nullptr) {
        DrawText("SELECTED PLANT", 20, yPos, 18, accentColor);
        yPos += 30;

        std::string plantNameText = selectedPlant->getName();
        DrawText(plantNameText.c_str(), 20, yPos, 16, textColor);
        yPos += 25;

        std::string idText = "ID: " + selectedPlant->getID();
        DrawText(idText.c_str(), 20, yPos, 13, detailColor);
        yPos += 25;

        std::string stateText = "State: " + selectedPlant->getState()->getStateName();
        DrawText(stateText.c_str(), 20, yPos, 13, detailColor);
        yPos += 25;

        std::ostringstream ageStream;
        ageStream << "Age: " << selectedPlant->getAge() << " days";
        DrawText(ageStream.str().c_str(), 20, yPos, 13, detailColor);
        yPos += 35;

        DrawLine(20, yPos, leftPanelWidth - 20, yPos, separatorColor);
        yPos += 20;

        DrawText("GROWTH STATUS:", 20, yPos, 14, headerColor);
        yPos += 30;

        std::string readyText = selectedPlant->isReadyForSale() ? "✓ Ready for Sale" : "✗ Still Growing";
        Color readyGood = manager->IsAlternativeColors() ? Color{150, 220, 150, 255} : Color{120, 165, 120, 255};
        Color readyBad = manager->IsAlternativeColors() ? Color{255, 150, 130, 255} : Color{235, 186, 170, 255};
        Color readyColor = selectedPlant->isReadyForSale() ? readyGood : readyBad;
        DrawText(readyText.c_str(), 20, yPos, 16, readyColor);
        yPos += 30;

        if (!selectedPlant->isReadyForSale()) {
            DrawText("This plant is still", 20, yPos, 12, detailColor);
            yPos += 18;
            DrawText("being cared for by", 20, yPos, 12, detailColor);
            yPos += 18;
            DrawText("our staff.", 20, yPos, 12, detailColor);
        }

    } else {
        DrawText("INSTRUCTIONS:", 20, yPos, 16, accentColor);
        yPos += 30;

        DrawText("1. Click a plant to view", 20, yPos, 13, textColor);
        yPos += 22;
        DrawText("2. Check plant details", 20, yPos, 13, textColor);
        yPos += 22;
        DrawText("3. See growth stages", 20, yPos, 13, textColor);
        yPos += 22;
        DrawText("4. Monitor plant health", 20, yPos, 13, textColor);
    }
}

void CustomerGreenhouseScreen::DrawMiddlePanel() {
    Color middleBg = manager->IsAlternativeColors()
        ? Color{35, 45, 40, 255}     // Dark mint
        : Color{206, 237, 223, 255}; // Soft mint
    Color lineColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    Color headerColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green

    DrawRectangle(leftPanelWidth, 0, middlePanelWidth, screenHeight, middleBg);
    DrawLine(leftPanelWidth + middlePanelWidth, 0, leftPanelWidth + middlePanelWidth, screenHeight, lineColor);

    const char* header = "CUSTOMER GREENHOUSE - GROWING AREA";
    int headerSize = 22;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header,
             leftPanelWidth + (middlePanelWidth - headerWidth) / 2,
             20,
             headerSize,
             headerColor);

    DrawGrid();
}

void CustomerGreenhouseScreen::DrawRightPanel() {
    Color rightBg = manager->IsAlternativeColors()
        ? Color{50, 45, 40, 255}     // Dark peachy
        : Color{255, 236, 214, 255}; // Peachy cream
    Color headerColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    Color emptyColor = manager->IsAlternativeColors()
        ? Color{180, 200, 190, 255}  // Light grey-green
        : Color{120, 140, 125, 255}; // Medium sage
    Color textColor = manager->IsAlternativeColors()
        ? Color{200, 200, 200, 255}  // Light grey
        : Color{85, 107, 95, 255};   // Dark forest green
    Color priceColor = manager->IsAlternativeColors()
        ? Color{255, 150, 130, 255}  // Light coral
        : Color{235, 186, 170, 255}; // Warm terracotta
    Color lineColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    Color totalColor = manager->IsAlternativeColors()
        ? Color{150, 220, 150, 255}  // Light green
        : Color{120, 165, 120, 255}; // Soft green

    DrawRectangle(leftPanelWidth + middlePanelWidth, 0, rightPanelWidth, screenHeight, rightBg);

    const char* header = "CART";
    int headerSize = 18;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header,
             screenWidth - rightPanelWidth + (rightPanelWidth - headerWidth) / 2,
             20,
             headerSize,
             headerColor);

    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;

    std::vector<Plant*> cart = customer->getCart();
    int yPos = 60;

    if (cart.empty()) {
        DrawText("Cart is empty", screenWidth - rightPanelWidth + 20, yPos, 16, emptyColor);
    } else {
        for (size_t i = 0; i < cart.size(); i++) {
            Plant* plant = cart[i];
            if (plant != nullptr) {
                std::string itemText = "- " + plant->getName();
                DrawText(itemText.c_str(), screenWidth - rightPanelWidth + 20, yPos, 14, textColor);
                yPos += 25;

                std::ostringstream priceStream;
                priceStream << "  R" << std::fixed << std::setprecision(2) << plant->getPrice();
                DrawText(priceStream.str().c_str(), screenWidth - rightPanelWidth + 30, yPos, 12, priceColor);
                yPos += 30;
            }
        }

        yPos += 10;
        DrawLine(screenWidth - rightPanelWidth + 20, yPos, screenWidth - 20, yPos, lineColor);
        yPos += 15;

        std::ostringstream totalStream;
        totalStream << "Total: R" << std::fixed << std::setprecision(2) << customer->calculateTotal();
        DrawText(totalStream.str().c_str(), screenWidth - rightPanelWidth + 20, yPos, 16, totalColor);
    }

    DrawButtons();
}

void CustomerGreenhouseScreen::DrawGrid() {
    Greenhouse* greenhouse = manager->GetGreenhouse();
    if (greenhouse == nullptr) return;

    Color cellIdle = manager->IsAlternativeColors()
        ? Color{50, 55, 50, 255}     // Dark sage
        : Color{245, 250, 247, 255}; // Very light sage
    Color cellSelected = manager->IsAlternativeColors()
        ? Color{80, 80, 60, 255}     // Dark yellow
        : Color{255, 247, 204, 255}; // Soft butter yellow
    Color cellBorder = manager->IsAlternativeColors()
        ? Color{80, 90, 85, 255}     // Grey-green
        : Color{200, 210, 205, 255}; // Light sage

    for (int row = 0; row < gridRows; row++) {
        for (int col = 0; col < gridCols; col++) {
            int x = gridStartX + col * cellSize;
            int y = gridStartY + row * cellSize;

            Color cellColor = cellIdle;
            if (row == selectedRow && col == selectedCol) {
                cellColor = cellSelected;
            }

            DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, cellColor);
            DrawRectangleLines(x + 1, y + 1, cellSize - 2, cellSize - 2, cellBorder);

            Plant* plant = greenhouse->getPlantAt(row, col);
            if (plant != nullptr) {
                DrawPlantInCell(plant, row, col);
            }
        }
    }
}

void CustomerGreenhouseScreen::DrawPlantInCell(Plant* plant, int row, int col) {
    int x = gridStartX + col * cellSize;
    int y = gridStartY + row * cellSize;
    
    // Get plant texture
    Texture2D plantTexture = manager->GetPlantTexture(plant->getName());
    
    if (plantTexture.id != 0) {
        // Calculate scaling to fit in cell
        int maxSize = cellSize - 20;
        float scale = static_cast<float>(maxSize) / plantTexture.height;
        
        int scaledWidth = static_cast<int>(plantTexture.width * scale);
        int scaledHeight = static_cast<int>(plantTexture.height * scale);
        
        // Center texture in cell
        int texX = x + (cellSize - scaledWidth) / 2;
        int texY = y + (cellSize - scaledHeight) / 2 - 5;
        
        // Draw with full color
        DrawTextureEx(plantTexture, Vector2{static_cast<float>(texX), static_cast<float>(texY)}, 0.0f, scale, WHITE);
    } else {
        Color nameColor = manager->IsAlternativeColors()
            ? Color{200, 200, 200, 255}  // Light grey
            : Color{85, 107, 95, 255};   // Dark forest green
        const char* name = plant->getName().c_str();
        int nameWidth = MeasureText(name, 10);
        DrawText(name, x + (cellSize - nameWidth) / 2, y + cellSize / 2 - 10, 10, nameColor);
    }

    Color stageGood = manager->IsAlternativeColors() ? Color{150, 220, 150, 255} : Color{120, 165, 120, 255};
    Color stageBad = manager->IsAlternativeColors() ? Color{255, 150, 130, 255} : Color{235, 186, 170, 255};
    std::string stage = plant->getState()->getStateName();
    int stageWidth = MeasureText(stage.c_str(), 11);
    Color stageColor = plant->isReadyForSale() ? stageGood : stageBad;
    DrawText(stage.c_str(), x + (cellSize - stageWidth) / 2, y + cellSize - 18, 11, stageColor);
}

void CustomerGreenhouseScreen::DrawButtons() {
    Customer* customer = manager->GetCustomer();
    bool hasCartItems = (customer != nullptr && customer->getCartSize() > 0);

    Color border = manager->IsAlternativeColors() ? Color{150, 220, 180, 255} : Color{85, 107, 95, 255};
    Color textColor = manager->IsAlternativeColors() ? Color{240, 240, 240, 255} : Color{85, 107, 95, 255};

    // Back to Sales Floor button
    Color backIdle = manager->IsAlternativeColors() ? Color{80, 50, 50, 255} : Color{245, 215, 220, 255};
    Color backHover = manager->IsAlternativeColors() ? Color{100, 65, 65, 255} : Color{255, 200, 195, 255};
    Color backColor = backButtonHovered ? backHover : backIdle;
    DrawRectangleRec(backToSalesFloorButton, backColor);
    DrawRectangleLinesEx(backToSalesFloorButton, 2, border);
    const char* backText = "Back to Sales Floor";
    int backTextWidth = MeasureText(backText, 16);
    DrawText(backText,
             backToSalesFloorButton.x + (backToSalesFloorButton.width - backTextWidth) / 2,
             backToSalesFloorButton.y + (backToSalesFloorButton.height - 16) / 2,
             16,
             textColor);

    // View Cart button
    Color cartIdle = manager->IsAlternativeColors()
        ? Color{50, 65, 75, 255}     // Dark blue-grey (active)
        : Color{220, 237, 245, 255}; // Light sky blue
    Color cartHover = manager->IsAlternativeColors()
        ? Color{60, 80, 90, 255}     // Lighter on hover
        : Color{210, 225, 240, 255}; // Sky blue hover
    Color cartDisabled = manager->IsAlternativeColors()
        ? Color{45, 45, 50, 255}     // Dark grey (disabled)
        : Color{200, 210, 205, 255}; // Light sage disabled
    Color cartColor = hasCartItems ? (viewCartHovered ? cartHover : cartIdle) : cartDisabled;
    DrawRectangleRec(viewCartButton, cartColor);
    DrawRectangleLinesEx(viewCartButton, 2, border);

    const char* cartText = "View Cart";
    int cartTextWidth = MeasureText(cartText, 18);
    Color cartTextColor = manager->IsAlternativeColors()
        ? (hasCartItems ? Color{240, 240, 240, 255} : Color{120, 120, 120, 255})  // Light or muted grey
        : (hasCartItems ? Color{85, 107, 95, 255} : Color{150, 160, 155, 255});   // Dark or muted
    DrawText(cartText,
             viewCartButton.x + (viewCartButton.width - cartTextWidth) / 2,
             viewCartButton.y + (viewCartButton.height - 18) / 2,
             18,
             cartTextColor);
}
