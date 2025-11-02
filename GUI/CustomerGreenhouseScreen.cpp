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
    ClearBackground(GRAY);
    
    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();
}

void CustomerGreenhouseScreen::DrawLeftPanel() {
    // Draw panel background - matches staff greenhouse
    DrawRectangle(0, 0, leftPanelWidth, screenHeight, Color{40, 40, 50, 255});
    DrawLine(leftPanelWidth, 0, leftPanelWidth, screenHeight, BLACK);
    
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;
    
    int yPos = 20;
    
    // Draw customer info
    DrawText("CUSTOMER INFO", 20, yPos, 20, WHITE);
    yPos += 35;
    
    // Customer name
    std::string nameText = "Name: " + customer->getName();
    DrawText(nameText.c_str(), 20, yPos, 16, LIGHTGRAY);
    yPos += 25;
    
    // Budget
    std::ostringstream budgetStream;
    budgetStream << "Budget: R" << std::fixed << std::setprecision(2) << customer->getBudget();
    DrawText(budgetStream.str().c_str(), 20, yPos, 18, GREEN);
    yPos += 45;
    
    // Draw separator line
    DrawLine(20, yPos, leftPanelWidth - 20, yPos, WHITE);
    yPos += 15;
    
    // Selected plant information
    if (selectedPlant != nullptr) {
        DrawText("SELECTED PLANT", 20, yPos, 18, YELLOW);
        yPos += 30;
        
        // Plant name
        std::string plantNameText = selectedPlant->getName();
        DrawText(plantNameText.c_str(), 20, yPos, 16, WHITE);
        yPos += 25;
        
        // Plant ID
        std::string idText = "ID: " + selectedPlant->getID();
        DrawText(idText.c_str(), 20, yPos, 13, LIGHTGRAY);
        yPos += 25;
        
        // Plant state
        std::string stateText = "State: " + selectedPlant->getState()->getStateName();
        DrawText(stateText.c_str(), 20, yPos, 13, LIGHTGRAY);
        yPos += 25;
        
        // Plant age
        std::ostringstream ageStream;
        ageStream << "Age: " << selectedPlant->getAge() << " days";
        DrawText(ageStream.str().c_str(), 20, yPos, 13, LIGHTGRAY);
        yPos += 35;
        
        DrawLine(20, yPos, leftPanelWidth - 20, yPos, DARKGRAY);
        yPos += 20;
        
        // Growth status
        DrawText("GROWTH STATUS:", 20, yPos, 14, SKYBLUE);
        yPos += 30;
        
        // Ready for sale status
        std::string readyText = selectedPlant->isReadyForSale() ? "✓ Ready for Sale" : "✗ Still Growing";
        Color readyColor = selectedPlant->isReadyForSale() ? GREEN : ORANGE;
        DrawText(readyText.c_str(), 20, yPos, 16, readyColor);
        yPos += 30;
        
        // Add a note about viewing more details
        if (!selectedPlant->isReadyForSale()) {
            DrawText("This plant is still", 20, yPos, 12, LIGHTGRAY);
            yPos += 18;
            DrawText("being cared for by", 20, yPos, 12, LIGHTGRAY);
            yPos += 18;
            DrawText("our staff.", 20, yPos, 12, LIGHTGRAY);
        }
        
    } else {
        // No plant selected - show instructions
        DrawText("INSTRUCTIONS:", 20, yPos, 16, YELLOW);
        yPos += 30;
        
        DrawText("1. Click a plant to view", 20, yPos, 13, WHITE);
        yPos += 22;
        DrawText("2. Check plant details", 20, yPos, 13, WHITE);
        yPos += 22;
        DrawText("3. See growth stages", 20, yPos, 13, WHITE);
        yPos += 22;
        DrawText("4. Monitor plant health", 20, yPos, 13, WHITE);
    }
}

void CustomerGreenhouseScreen::DrawMiddlePanel() {
    // Draw panel background - matches staff greenhouse
    DrawRectangle(leftPanelWidth, 0, middlePanelWidth, screenHeight, Color{60, 80, 70, 255});
    DrawLine(leftPanelWidth + middlePanelWidth, 0, leftPanelWidth + middlePanelWidth, screenHeight, BLACK);
    
    // Draw header
    const char* header = "CUSTOMER GREENHOUSE - GROWING AREA";
    int headerSize = 22;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header, 
             leftPanelWidth + (middlePanelWidth - headerWidth) / 2, 
             20, 
             headerSize, 
             WHITE);
    
    // Draw grid
    DrawGrid();
}

void CustomerGreenhouseScreen::DrawRightPanel() {
    // Draw panel background - matches staff greenhouse
    DrawRectangle(leftPanelWidth + middlePanelWidth, 0, rightPanelWidth, screenHeight, Color{30, 50, 40, 255});
    
    // Draw header
    const char* header = "CART";
    int headerSize = 18;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header, 
             screenWidth - rightPanelWidth + (rightPanelWidth - headerWidth) / 2, 
             20, 
             headerSize, 
             WHITE);
    
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;
    
    // Draw cart items
    std::vector<Plant*> cart = customer->getCart();
    int yPos = 60;
    
    if (cart.empty()) {
        DrawText("Cart is empty", screenWidth - rightPanelWidth + 20, yPos, 16, LIGHTGRAY);
    } else {
        for (size_t i = 0; i < cart.size(); i++) {
            Plant* plant = cart[i];
            if (plant != nullptr) {
                std::string itemText = "- " + plant->getName();
                DrawText(itemText.c_str(), screenWidth - rightPanelWidth + 20, yPos, 14, WHITE);
                yPos += 25;
                
                std::ostringstream priceStream;
                priceStream << "  R" << std::fixed << std::setprecision(2) << plant->getPrice();
                DrawText(priceStream.str().c_str(), screenWidth - rightPanelWidth + 30, yPos, 12, YELLOW);
                yPos += 30;
            }
        }
        
        // Draw total
        yPos += 10;
        DrawLine(screenWidth - rightPanelWidth + 20, yPos, screenWidth - 20, yPos, WHITE);
        yPos += 15;
        
        std::ostringstream totalStream;
        totalStream << "Total: R" << std::fixed << std::setprecision(2) << customer->calculateTotal();
        DrawText(totalStream.str().c_str(), screenWidth - rightPanelWidth + 20, yPos, 16, GREEN);
    }
    
    // Draw buttons
    DrawButtons();
}

void CustomerGreenhouseScreen::DrawGrid() {
    Greenhouse* greenhouse = manager->GetGreenhouse();
    if (greenhouse == nullptr) return;
    
    for (int row = 0; row < gridRows; row++) {
        for (int col = 0; col < gridCols; col++) {
            int x = gridStartX + col * cellSize;
            int y = gridStartY + row * cellSize;
            
            // Determine cell color - matches staff greenhouse
            Color cellColor = Color{50, 70, 60, 255};
            if (row == selectedRow && col == selectedCol) {
                cellColor = GOLD;
            }
            
            // Draw cell background
            DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, cellColor);
            DrawRectangleLines(x + 1, y + 1, cellSize - 2, cellSize - 2, Color{40, 60, 50, 255});
            
            // Draw plant if exists
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
        // Fallback: draw plant name
        const char* name = plant->getName().c_str();
        int nameWidth = MeasureText(name, 10);
        DrawText(name, x + (cellSize - nameWidth) / 2, y + cellSize / 2 - 10, 10, DARKGREEN);
    }
    
    // Draw growth stage indicator at bottom of cell (no health bar for customers)
    std::string stage = plant->getState()->getStateName();
    int stageWidth = MeasureText(stage.c_str(), 11);
    Color stageColor = plant->isReadyForSale() ? GREEN : ORANGE;
    DrawText(stage.c_str(), x + (cellSize - stageWidth) / 2, y + cellSize - 18, 11, stageColor);
}

void CustomerGreenhouseScreen::DrawButtons() {
    Customer* customer = manager->GetCustomer();
    bool hasCartItems = (customer != nullptr && customer->getCartSize() > 0);
    
    // Back to Sales Floor button
    Color backColor = backButtonHovered ? DARKGRAY : Color{100, 100, 100, 255};
    DrawRectangleRec(backToSalesFloorButton, backColor);
    DrawRectangleLinesEx(backToSalesFloorButton, 2, BLACK);
    const char* backText = "Back to Sales Floor";
    int backTextWidth = MeasureText(backText, 16);
    DrawText(backText,
             backToSalesFloorButton.x + (backToSalesFloorButton.width - backTextWidth) / 2,
             backToSalesFloorButton.y + (backToSalesFloorButton.height - 16) / 2,
             16,
             WHITE);
    
    // View Cart button
    Color cartColor = hasCartItems ? 
                      (viewCartHovered ? Color{0, 120, 200, 255} : Color{50, 150, 220, 255}) : GRAY;
    DrawRectangleRec(viewCartButton, cartColor);
    DrawRectangleLinesEx(viewCartButton, 2, BLACK);
    const char* cartText = "View Cart";
    int cartTextWidth = MeasureText(cartText, 18);
    DrawText(cartText,
             viewCartButton.x + (viewCartButton.width - cartTextWidth) / 2,
             viewCartButton.y + (viewCartButton.height - 18) / 2,
             18,
             WHITE);
}
