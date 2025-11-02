#include "SalesFloorScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

// Backend includes
#include "../include/Customer.h"
#include "../include/SalesFloor.h"
#include "../include/Plant.h"
#include "../include/Request.h"
#include "../include/SalesAssistant.h"
#include "../include/NurseryCoordinator.h" 

SalesFloorScreen::SalesFloorScreen(ScreenManager* mgr)
    : manager(mgr),
      selectedRow(-1),
      selectedCol(-1),
      selectedPlant(nullptr),
      addToCartHovered(false),
      viewGreenhouseHovered(false),
      viewCartHovered(false),
      createOrderHovered(false),
      makeRequestHovered(false),
      backToStartHovered(false),
      requestOverlayActive(false),
      requestTextLength(0),
      responseText("") {
    
    gridRows = 5;
    gridCols = 5;
    
    InitializeLayout();
    InitializeButtons();
    
    std::memset(requestText, 0, sizeof(requestText));
}

SalesFloorScreen::~SalesFloorScreen() {
}

void SalesFloorScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    leftPanelWidth = screenWidth / 4;
    middlePanelWidth = screenWidth / 2;
    rightPanelWidth = screenWidth / 4;
    
    int availableWidth = middlePanelWidth - 40;
    int availableHeight = screenHeight - 100;
    
    int cellSizeByWidth = availableWidth / gridCols;
    int cellSizeByHeight = availableHeight / gridRows;
    
    cellSize = (cellSizeByWidth < cellSizeByHeight) ? cellSizeByWidth : cellSizeByHeight;
    
    int gridTotalWidth = cellSize * gridCols;
    int gridTotalHeight = cellSize * gridRows;
    
    gridStartX = leftPanelWidth + (middlePanelWidth - gridTotalWidth) / 2;
    gridStartY = 80 + (screenHeight - 80 - gridTotalHeight) / 2;
}

void SalesFloorScreen::InitializeButtons() {
    int buttonWidth = rightPanelWidth - 40;
    int buttonHeight = 45;
    int buttonSpacing = 15;
    int buttonX = screenWidth - rightPanelWidth + 20;
    int startY = screenHeight - 400;  // CHANGED: moved up more to accommodate all buttons
    
    addToCartButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    viewGreenhouseButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + buttonHeight + buttonSpacing),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
    
    viewCartButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 2),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    createOrderButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 3),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    makeRequestButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 4),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    // NEW: Back to Start button
    backToStartButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 5),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
}

void SalesFloorScreen::Update() {
    if (requestOverlayActive) {
        UpdateRequestOverlay();
    } else {
        if (selectedPlant != nullptr) {
            SalesFloor* salesFloor = manager->GetSalesFloor();
            if (salesFloor != nullptr) {
                Plant* currentPlant = salesFloor->getPlantAt(selectedRow, selectedCol);
                if (currentPlant != selectedPlant) {
                    selectedPlant = nullptr;
                    selectedRow = -1;
                    selectedCol = -1;
                }
            }
        }
        
        UpdateGrid();
        UpdateButtons();
    }
}

void SalesFloorScreen::UpdateGrid() {
    Vector2 mousePos = GetMousePosition();
    
    if (mousePos.x >= gridStartX && mousePos.x < gridStartX + (cellSize * gridCols) &&
        mousePos.y >= gridStartY && mousePos.y < gridStartY + (cellSize * gridRows)) {
        
        int col = static_cast<int>((mousePos.x - gridStartX) / cellSize);
        int row = static_cast<int>((mousePos.y - gridStartY) / cellSize);
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            HandlePlantSelection(row, col);
        }
    }
}

void SalesFloorScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();

    addToCartHovered = CheckCollisionPointRec(mousePos, addToCartButton);
    viewGreenhouseHovered = CheckCollisionPointRec(mousePos, viewGreenhouseButton);
    viewCartHovered = CheckCollisionPointRec(mousePos, viewCartButton);
    createOrderHovered = CheckCollisionPointRec(mousePos, createOrderButton);
    makeRequestHovered = CheckCollisionPointRec(mousePos, makeRequestButton);
    backToStartHovered = CheckCollisionPointRec(mousePos, backToStartButton);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (addToCartHovered && selectedPlant != nullptr) {
            HandleAddToCart();
        } else if (viewGreenhouseHovered) {
            manager->SwitchScreen(GameScreen::CUSTOMER_GREENHOUSE);
        } else if (viewCartHovered) {
            Customer* customer = manager->GetCustomer();
            if (customer != nullptr && customer->getCartSize() > 0) {
                manager->SwitchScreen(GameScreen::CART_VIEW);
            }
        } else if (createOrderHovered) {
            HandleCreateOrder();
        } else if (makeRequestHovered) {
            requestOverlayActive = true;
            requestTextLength = 0;
            std::memset(requestText, 0, sizeof(requestText));
            responseText = "";
        } else if (backToStartHovered) {
            HandleBackToStart();
        }
    }
}

void SalesFloorScreen::UpdateRequestOverlay() {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (requestTextLength < 255)) {
            requestText[requestTextLength] = static_cast<char>(key);
            requestTextLength++;
            requestText[requestTextLength] = '\0';
        }
        key = GetCharPressed();
    }
    
    if (IsKeyPressed(KEY_BACKSPACE) && requestTextLength > 0) {
        requestTextLength--;
        requestText[requestTextLength] = '\0';
    }
    
    if (IsKeyPressed(KEY_ENTER) && requestTextLength > 0) {
        HandleMakeRequest();
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        requestOverlayActive = false;
    }
}

void SalesFloorScreen::HandlePlantSelection(int row, int col) {
    SalesFloor* salesFloor = manager->GetSalesFloor();
    if (salesFloor == nullptr) return;
    
    Plant* plant = salesFloor->getPlantAt(row, col);
    
    if (plant != nullptr) {
        selectedRow = row;
        selectedCol = col;
        selectedPlant = plant;
        std::cout << "[SalesFloorScreen] Selected plant: " << plant->getName() 
                  << " at (" << row << "," << col << ")" << std::endl;
    } else {
        selectedRow = -1;
        selectedCol = -1;
        selectedPlant = nullptr;
    }
}

void SalesFloorScreen::HandleAddToCart() {
    if (selectedPlant == nullptr) return;
    
    Customer* customer = manager->GetCustomer();
    SalesFloor* salesFloor = manager->GetSalesFloor();
    NurseryCoordinator* mediator = manager->GetMediator();
    
    if (customer == nullptr || salesFloor == nullptr || mediator == nullptr) return;
    
    std::string plantName = selectedPlant->getName();
    std::string plantID = selectedPlant->getID();
    
    std::cout << "[SalesFloorScreen] Attempting to add " << plantName 
              << " (ID: " << plantID << ") at position (" << selectedRow << "," << selectedCol << ") to cart" << std::endl;
    
    bool success = mediator->transferPlantFromPosition(selectedRow, selectedCol, customer);
    
    if (success) {
        std::cout << "[SalesFloorScreen] Successfully added " << plantName << " to cart" << std::endl;
        selectedRow = -1;
        selectedCol = -1;
        selectedPlant = nullptr;
    } else {
        std::cout << "[SalesFloorScreen] Failed to add plant to cart" << std::endl;
    }
}

void SalesFloorScreen::HandleCreateOrder() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) {
        std::cout << "[SalesFloorScreen] No customer available" << std::endl;
        return;
    }

    if (customer->getCartSize() == 0) {
        std::cout << "[SalesFloorScreen] Cart is empty, cannot create order" << std::endl;
        return;
    }

    std::cout << "[SalesFloorScreen] Navigating to order creation screen" << std::endl;
    manager->SwitchScreen(GameScreen::ORDER_CREATION);
}

void SalesFloorScreen::HandleMakeRequest() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;
    
    Request* request = customer->createRequest(std::string(requestText));
    
    SalesAssistant* salesAssistant = manager->GetSalesAssistant();
    if (salesAssistant != nullptr) {
        salesAssistant->handleRequest(request);
        
        if (request->isHandled()) {
            responseText = "Request handled: " + request->getMessage();
            
            std::string plantName = request->extractPlantName();
            if (!plantName.empty()) {
                responseText += "\n\nPlant '" + plantName + "' has been added to your cart!";
            }
        } else {
            responseText = "Request could not be handled at this time.";
        }
    }
}

// NEW: Handle back to start screen
void SalesFloorScreen::HandleBackToStart() {
    std::cout << "[SalesFloorScreen] Returning to start screen" << std::endl;
    
    // Optional: Delete customer when going back
    manager->DeleteCustomer();
    
    manager->SwitchScreen(GameScreen::START);
}

void SalesFloorScreen::Draw() {
    // Light mode: Soft sage green | Dark mode: Dark background
    Color bgColor = manager->IsAlternativeColors()
        ? Color{30, 30, 35, 255}     // Dark mode
        : Color{216, 228, 220, 255}; // Light sage green
    ClearBackground(bgColor);

    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();

    if (requestOverlayActive) {
        DrawRequestOverlay();
    }
}

void SalesFloorScreen::DrawLeftPanel() {
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
    Color waterColor = manager->IsAlternativeColors()
        ? Color{130, 170, 220, 255}  // Light blue
        : Color{50, 120, 160, 255};  // Darker blue
    Color nutrientColor = manager->IsAlternativeColors()
        ? Color{150, 220, 150, 255}  // Light green
        : Color{120, 165, 120, 255}; // Soft green
    Color priceColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green

    if (selectedPlant != nullptr) {
        DrawText("SELECTED PLANT", 20, yPos, 20, accentColor);
        yPos += 35;

        std::string plantNameText = selectedPlant->getName();
        DrawText(plantNameText.c_str(), 20, yPos, 18, textColor);
        yPos += 30;

        std::string idText = "ID: " + selectedPlant->getID();
        DrawText(idText.c_str(), 20, yPos, 14, detailColor);
        yPos += 25;

        std::string stateText = "State: " + selectedPlant->getState()->getStateName();
        DrawText(stateText.c_str(), 20, yPos, 14, detailColor);
        yPos += 25;

        std::ostringstream ageStream;
        ageStream << "Age: " << selectedPlant->getAge() << " days";
        DrawText(ageStream.str().c_str(), 20, yPos, 14, detailColor);
        yPos += 25;

        std::ostringstream healthStream;
        healthStream << "Health: " << selectedPlant->getHealthLevel() << "%";
        Color healthGood = manager->IsAlternativeColors() ? Color{150, 220, 150, 255} : Color{120, 165, 120, 255};
        Color healthMed = manager->IsAlternativeColors() ? Color{255, 200, 130, 255} : Color{235, 186, 170, 255};
        Color healthBad = manager->IsAlternativeColors() ? Color{255, 120, 120, 255} : Color{200, 100, 100, 255};
        Color healthColor = selectedPlant->getHealthLevel() > 70 ? healthGood :
                           selectedPlant->getHealthLevel() > 40 ? healthMed : healthBad;
        DrawText(healthStream.str().c_str(), 20, yPos, 14, healthColor);
        yPos += 25;

        std::ostringstream waterStream;
        waterStream << "Water: " << selectedPlant->getWaterLevel() << "%";
        DrawText(waterStream.str().c_str(), 20, yPos, 14, waterColor);
        yPos += 25;

        std::ostringstream nutrientStream;
        nutrientStream << "Nutrients: " << selectedPlant->getNutrientLevel() << "%";
        DrawText(nutrientStream.str().c_str(), 20, yPos, 14, nutrientColor);
        yPos += 30;

        std::ostringstream priceStream;
        priceStream << "Price: R" << std::fixed << std::setprecision(2) << selectedPlant->getPrice();
        DrawText(priceStream.str().c_str(), 20, yPos, 24, priceColor);
        yPos += 40;

    } else {
        DrawText("INSTRUCTIONS:", 20, yPos, 18, accentColor);
        yPos += 30;

        DrawText("- Click plants to select", 20, yPos, 16, textColor);
        yPos += 25;
        DrawText("- View plant details here", 20, yPos, 16, textColor);
        yPos += 25;
        DrawText("- Add to cart when ready", 20, yPos, 16, textColor);
        yPos += 25;
        DrawText("- Use buttons to navigate", 20, yPos, 16, textColor);
    }
}

void SalesFloorScreen::DrawMiddlePanel() {
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

    const char* header = "SALES FLOOR";
    int headerSize = 24;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header,
             leftPanelWidth + (middlePanelWidth - headerWidth) / 2,
             20,
             headerSize,
             headerColor);

    DrawGrid();
}

void SalesFloorScreen::DrawRightPanel() {
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
        ? Color{160, 160, 160, 255}  // Grey
        : Color{100, 100, 100, 255}; // Medium grey
    Color totalColor = manager->IsAlternativeColors()
        ? Color{120, 220, 150, 255}  // Light green
        : GREEN;

    DrawRectangle(leftPanelWidth + middlePanelWidth, 0, rightPanelWidth, screenHeight, rightBg);

    const char* header = "CART";
    int headerSize = 20;
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
        Vector2 mousePos = GetMousePosition();

        for (size_t i = 0; i < cart.size(); i++) {
            Plant* plant = cart[i];
            if (plant != nullptr) {
                int removeX = screenWidth - rightPanelWidth + 20;
                int removeY = yPos;
                Rectangle removeBtn = Rectangle{
                    static_cast<float>(removeX),
                    static_cast<float>(removeY),
                    20,
                    20
                };

                bool removeHovered = CheckCollisionPointRec(mousePos, removeBtn);
                Color removeIdle = manager->IsAlternativeColors() ? Color{150, 70, 70, 255} : Color{200, 100, 100, 255};
                Color removeHover = manager->IsAlternativeColors() ? Color{180, 90, 90, 255} : Color{255, 150, 150, 255};
                Color removeBtnColor = removeHovered ? removeHover : removeIdle;
                Color removeBorder = manager->IsAlternativeColors() ? Color{150, 220, 180, 255} : Color{85, 107, 95, 255};
                DrawRectangleRec(removeBtn, removeBtnColor);
                DrawRectangleLinesEx(removeBtn, 1, removeBorder);
                DrawText("X", removeX + 6, removeY + 3, 14, WHITE);

                if (removeHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    std::cout << "[SalesFloorScreen] Removing item " << i << " from cart" << std::endl;
                    customer->returnPlantToSalesFloor((int)i);
                    //break;
                    return;
                }

                std::string itemText = plant->getName();
                DrawText(itemText.c_str(), screenWidth - rightPanelWidth + 45, yPos + 3, 14, textColor);
                yPos += 25;

                std::ostringstream priceStream;
                priceStream << "  R" << std::fixed << std::setprecision(2) << plant->getPrice();
                DrawText(priceStream.str().c_str(), screenWidth - rightPanelWidth + 30, yPos, 12, priceColor);
                yPos += 30;
            }
        }

        yPos += 10;
        DrawLine(screenWidth - rightPanelWidth + 20, yPos, screenWidth - 20, yPos, WHITE);
        yPos += 15;

        double cartTotal = 0.0;
        for (Plant* p : cart) {
            if (p != nullptr) {
                cartTotal += p->getPrice();
            }
        }

        std::ostringstream totalStream;
        totalStream << "Total: R" << std::fixed << std::setprecision(2) << cartTotal;
        DrawText(totalStream.str().c_str(), screenWidth - rightPanelWidth + 20, yPos, 16, totalColor);
    }

    DrawButtons();
}

void SalesFloorScreen::DrawGrid() {
    SalesFloor* salesFloor = manager->GetSalesFloor();
    if (salesFloor == nullptr) return;

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

            DrawRectangle(x + 2, y + 2, cellSize - 4, cellSize - 4, cellColor);
            DrawRectangleLines(x + 2, y + 2, cellSize - 4, cellSize - 4, cellBorder);

            Plant* plant = salesFloor->getPlantAt(row, col);
            if (plant != nullptr) {
                DrawPlantInCell(plant, row, col);
            }
        }
    }
}

void SalesFloorScreen::DrawPlantInCell(Plant* plant, int row, int col) {
    int x = gridStartX + col * cellSize;
    int y = gridStartY + row * cellSize;
    
    Texture2D plantTexture = manager->GetPlantTexture(plant->getName());
    
    if (plantTexture.id != 0) {
        int maxSize = cellSize - 30;
        float scale = static_cast<float>(maxSize) / plantTexture.height;
        
        int scaledWidth = static_cast<int>(plantTexture.width * scale);
        int scaledHeight = static_cast<int>(plantTexture.height * scale);
        
        int texX = x + (cellSize - scaledWidth) / 2;
        int texY = y + (cellSize - scaledHeight) / 2 - 10;
        
        DrawTextureEx(plantTexture, Vector2{static_cast<float>(texX), static_cast<float>(texY)}, 0.0f, scale, WHITE);
    } else {
        Color nameColor = manager->IsAlternativeColors()
            ? Color{200, 200, 200, 255}  // Light grey
            : Color{85, 107, 95, 255};   // Dark forest green
        const char* name = plant->getName().c_str();
        int nameWidth = MeasureText(name, 12);
        DrawText(name, x + (cellSize - nameWidth) / 2, y + cellSize / 2 - 10, 12, nameColor);
    }

    Color priceColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    std::ostringstream priceStream;
    priceStream << "R" << static_cast<int>(plant->getPrice());
    std::string priceText = priceStream.str();
    int priceWidth = MeasureText(priceText.c_str(), 16);
    DrawText(priceText.c_str(), x + (cellSize - priceWidth) / 2, y + cellSize - 25, 16, priceColor);
}

void SalesFloorScreen::DrawButtons() {
    Customer* customer = manager->GetCustomer();
    bool hasCartItems = (customer != nullptr && customer->getCartSize() > 0);

    Color border = manager->IsAlternativeColors() ? Color{150, 220, 180, 255} : Color{85, 107, 95, 255};
    Color textActive = manager->IsAlternativeColors() ? Color{240, 240, 240, 255} : Color{85, 107, 95, 255};
    Color textMuted = manager->IsAlternativeColors() ? Color{120, 120, 120, 255} : Color{150, 160, 155, 255};
    Color disabled = manager->IsAlternativeColors() ? Color{60, 65, 60, 255} : Color{200, 210, 205, 255};

    // Add to Cart button
    Color addIdle = manager->IsAlternativeColors() ? Color{40, 80, 50, 255} : Color{120, 165, 120, 255};
    Color addHover = manager->IsAlternativeColors() ? Color{50, 100, 60, 255} : Color{100, 145, 100, 255};
    Color addColor = (selectedPlant != nullptr) ? (addToCartHovered ? addHover : addIdle) : disabled;
    DrawRectangleRec(addToCartButton, addColor);
    DrawRectangleLinesEx(addToCartButton, 2, border);
    const char* addText = "Add to Cart";
    int addTextWidth = MeasureText(addText, 18);
    DrawText(addText,
             addToCartButton.x + (addToCartButton.width - addTextWidth) / 2,
             addToCartButton.y + (addToCartButton.height - 18) / 2,
             18,
             (selectedPlant != nullptr) ? textActive : textMuted);

    // View Greenhouse button
    Color ghIdle = manager->IsAlternativeColors() ? Color{40, 50, 60, 255} : Color{220, 237, 245, 255};
    Color ghHover = manager->IsAlternativeColors() ? Color{50, 65, 75, 255} : Color{210, 224, 230, 255};
    Color greenhouseColor = viewGreenhouseHovered ? ghHover : ghIdle;
    DrawRectangleRec(viewGreenhouseButton, greenhouseColor);
    DrawRectangleLinesEx(viewGreenhouseButton, 2, border);
    const char* greenhouseText = "View Greenhouse";
    int greenhouseTextWidth = MeasureText(greenhouseText, 16);
    DrawText(greenhouseText,
             viewGreenhouseButton.x + (viewGreenhouseButton.width - greenhouseTextWidth) / 2,
             viewGreenhouseButton.y + (viewGreenhouseButton.height - 16) / 2,
             16,
             textActive);

    // View Cart button
    Color cartIdle = manager->IsAlternativeColors() ? Color{80, 50, 45, 255} : Color{235, 186, 170, 255};
    Color cartHover = manager->IsAlternativeColors() ? Color{100, 65, 55, 255} : Color{225, 176, 160, 255};
    Color cartColor = hasCartItems ? (viewCartHovered ? cartHover : cartIdle) : disabled;
    Color cartBorder = manager->IsAlternativeColors() ? border : BLACK;
    DrawRectangleRec(viewCartButton, cartColor);
    DrawRectangleLinesEx(viewCartButton, 2, cartBorder);
    const char* cartText = "View Cart";
    int cartTextWidth = MeasureText(cartText, 18);
    DrawText(cartText,
             viewCartButton.x + (viewCartButton.width - cartTextWidth) / 2,
             viewCartButton.y + (viewCartButton.height - 18) / 2,
             18,
             hasCartItems ? textActive : textMuted);

    // Create Order button
    Color orderIdle = manager->IsAlternativeColors() ? Color{70, 70, 50, 255} : Color{255, 247, 204, 255};
    Color orderHover = manager->IsAlternativeColors() ? Color{85, 85, 60, 255} : Color{245, 237, 194, 255};
    Color orderColor = hasCartItems ? (createOrderHovered ? orderHover : orderIdle) : disabled;
    Color orderBorder = manager->IsAlternativeColors() ? border : BLACK;
    DrawRectangleRec(createOrderButton, orderColor);
    DrawRectangleLinesEx(createOrderButton, 2, orderBorder);
    const char* orderText = "Create Order";
    int orderTextWidth = MeasureText(orderText, 18);
    DrawText(orderText,
             createOrderButton.x + (createOrderButton.width - orderTextWidth) / 2,
             createOrderButton.y + (createOrderButton.height - 18) / 2,
             18,
             hasCartItems ? textActive : textMuted);

    // Make Request button
    Color reqIdle = manager->IsAlternativeColors() ? Color{55, 45, 65, 255} : Color{230, 224, 237, 255};
    Color reqHover = manager->IsAlternativeColors() ? Color{70, 55, 85, 255} : Color{215, 195, 220, 255};
    Color requestColor = makeRequestHovered ? reqHover : reqIdle;
    Color reqBorder = manager->IsAlternativeColors() ? border : BLACK;
    DrawRectangleRec(makeRequestButton, requestColor);
    DrawRectangleLinesEx(makeRequestButton, 2, reqBorder);
    const char* requestText = "Make Request";
    int requestTextWidth = MeasureText(requestText, 16);
    DrawText(requestText,
             makeRequestButton.x + (makeRequestButton.width - requestTextWidth) / 2,
             makeRequestButton.y + (makeRequestButton.height - 16) / 2,
             16,
             textActive);

    // NEW: Back to Start button
    Color backIdle = manager->IsAlternativeColors() ? Color{80, 50, 50, 255} : Color{245, 215, 220, 255};
    Color backHover = manager->IsAlternativeColors() ? Color{100, 65, 65, 255} : Color{255, 200, 195, 255};
    Color backColor = backToStartHovered ? backHover : backIdle;
    Color backBorder = manager->IsAlternativeColors() ? border : BLACK;
    DrawRectangleRec(backToStartButton, backColor);
    DrawRectangleLinesEx(backToStartButton, 2, backBorder);
    const char* backText = "Back to Start";
    int backTextWidth = MeasureText(backText, 16);
    DrawText(backText,
             backToStartButton.x + (backToStartButton.width - backTextWidth) / 2,
             backToStartButton.y + (backToStartButton.height - 16) / 2,
             16,
             textActive);
}

void SalesFloorScreen::DrawRequestOverlay() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});
    
    int overlayWidth = 600;
    int overlayHeight = 400;
    int overlayX = (screenWidth - overlayWidth) / 2;
    int overlayY = (screenHeight - overlayHeight) / 2;
    
    DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, Color{40, 40, 60, 255});
    DrawRectangleLinesEx(Rectangle{static_cast<float>(overlayX), static_cast<float>(overlayY), 
                                   static_cast<float>(overlayWidth), static_cast<float>(overlayHeight)}, 
                        3, GOLD);
    
    const char* title = "Submit Request to Staff";
    int titleWidth = MeasureText(title, 24);
    DrawText(title, overlayX + (overlayWidth - titleWidth) / 2, overlayY + 20, 24, WHITE);
    
    int inputY = overlayY + 70;
    DrawRectangle(overlayX + 20, inputY, overlayWidth - 40, 40, WHITE);
    DrawRectangleLines(overlayX + 20, inputY, overlayWidth - 40, 40, BLACK);
    DrawText(requestText, overlayX + 30, inputY + 10, 20, BLACK);
    
    if (static_cast<int>(GetTime() * 2) % 2 == 0) {
        int cursorX = overlayX + 30 + MeasureText(requestText, 20);
        DrawText("|", cursorX, inputY + 10, 20, BLACK);
    }
    
    DrawText("Type your request and press ENTER to submit", 
             overlayX + 20, inputY + 50, 16, LIGHTGRAY);
    DrawText("Press ESC to cancel", 
             overlayX + 20, inputY + 75, 16, LIGHTGRAY);
    
    if (!responseText.empty()) {
        int responseY = inputY + 110;
        DrawText("Response:", overlayX + 20, responseY, 18, YELLOW);
        
        int maxWidth = overlayWidth - 60;
        int lineY = responseY + 30;
        std::istringstream iss(responseText);
        std::string word;
        std::string currentLine;
        
        while (iss >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            if (MeasureText(testLine.c_str(), 16) > maxWidth) {
                DrawText(currentLine.c_str(), overlayX + 30, lineY, 16, WHITE);
                lineY += 22;
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }
        if (!currentLine.empty()) {
            DrawText(currentLine.c_str(), overlayX + 30, lineY, 16, WHITE);
        }
    }
    
    Rectangle closeButton = Rectangle{
        static_cast<float>(overlayX + overlayWidth / 2 - 50),
        static_cast<float>(overlayY + overlayHeight - 60),
        100,
        40
    };
    
    Vector2 mousePos = GetMousePosition();
    bool closeHovered = CheckCollisionPointRec(mousePos, closeButton);
    
    Color closeColor = closeHovered ? DARKGRAY : GRAY;
    DrawRectangleRec(closeButton, closeColor);
    DrawRectangleLinesEx(closeButton, 2, BLACK);
    
    const char* closeText = "Close";
    int closeTextWidth = MeasureText(closeText, 20);
    DrawText(closeText,
             closeButton.x + (closeButton.width - closeTextWidth) / 2,
             closeButton.y + (closeButton.height - 20) / 2,
             20,
             WHITE);
    
    if (closeHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        requestOverlayActive = false;
    }
}