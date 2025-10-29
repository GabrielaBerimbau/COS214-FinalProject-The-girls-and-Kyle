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
      makeRequestHovered(false),
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
    // Nothing to clean up
}

void SalesFloorScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    // 3-column layout: 25% | 50% | 25%
    leftPanelWidth = screenWidth / 4;
    middlePanelWidth = screenWidth / 2;
    rightPanelWidth = screenWidth / 4;
    
    // Calculate grid cell size (fits in middle panel with padding)
    int availableWidth = middlePanelWidth - 40; // 20px padding each side
    int availableHeight = screenHeight - 100; // Leave space for header
    
    int cellSizeByWidth = availableWidth / gridCols;
    int cellSizeByHeight = availableHeight / gridRows;
    
    cellSize = (cellSizeByWidth < cellSizeByHeight) ? cellSizeByWidth : cellSizeByHeight;
    
    // Center the grid in the middle panel
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
    int startY = screenHeight - 250;
    
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
    
    makeRequestButton = Rectangle{
        static_cast<float>(buttonX),
        static_cast<float>(startY + (buttonHeight + buttonSpacing) * 3),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };
}

void SalesFloorScreen::Update() {
    if (requestOverlayActive) {
        UpdateRequestOverlay();
    } else {
        // CRITICAL: Validate selection before updating
        if (selectedPlant != nullptr) {
            SalesFloor* salesFloor = manager->GetSalesFloor();
            if (salesFloor != nullptr) {
                // Check if selected plant is still at the selected position
                Plant* currentPlant = salesFloor->getPlantAt(selectedRow, selectedCol);
                if (currentPlant != selectedPlant) {
                    // Plant has been removed (via request or other means)
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

void SalesFloorScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();
    
    // Update hover states
    addToCartHovered = CheckCollisionPointRec(mousePos, addToCartButton);
    viewGreenhouseHovered = CheckCollisionPointRec(mousePos, viewGreenhouseButton);
    viewCartHovered = CheckCollisionPointRec(mousePos, viewCartButton);
    makeRequestHovered = CheckCollisionPointRec(mousePos, makeRequestButton);
    
    // Handle button clicks
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
        } else if (makeRequestHovered) {
            requestOverlayActive = true;
            requestTextLength = 0;
            std::memset(requestText, 0, sizeof(requestText));
            responseText = "";
        }
    }
}

void SalesFloorScreen::UpdateRequestOverlay() {
    // Handle text input
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (requestTextLength < 255)) {
            requestText[requestTextLength] = static_cast<char>(key);
            requestTextLength++;
            requestText[requestTextLength] = '\0';
        }
        key = GetCharPressed();
    }
    
    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE) && requestTextLength > 0) {
        requestTextLength--;
        requestText[requestTextLength] = '\0';
    }
    
    // Handle submit (Enter key)
    if (IsKeyPressed(KEY_ENTER) && requestTextLength > 0) {
        HandleMakeRequest();
    }
    
    // Handle close (Escape key or click outside)
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
    
    // FIXED: Use position-based transfer instead of name-based
    bool success = mediator->transferPlantFromPosition(selectedRow, selectedCol, customer);
    
    if (success) {
        std::cout << "[SalesFloorScreen] Successfully added " << plantName << " to cart" << std::endl;
        
        // CRITICAL FIX: Clear selection to prevent dangling pointer
        selectedRow = -1;
        selectedCol = -1;
        selectedPlant = nullptr;
    } else {
        std::cout << "[SalesFloorScreen] Failed to add plant to cart" << std::endl;
    }
}

void SalesFloorScreen::HandleMakeRequest() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;
    
    // Create request
    Request* request = customer->createRequest(std::string(requestText));
    
    // Submit through chain of responsibility
    SalesAssistant* salesAssistant = manager->GetSalesAssistant();
    if (salesAssistant != nullptr) {
        salesAssistant->handleRequest(request);
        
        // Generate response text
        if (request->isHandled()) {
            responseText = "Request handled: " + request->getMessage();
            
            // If request was for a plant, it should now be in cart
            std::string plantName = request->extractPlantName();
            if (!plantName.empty()) {
                responseText += "\n\nPlant '" + plantName + "' has been added to your cart!";
            }
        } else {
            responseText = "Request could not be handled at this time.";
        }
    }

}

void SalesFloorScreen::Draw() {
    ClearBackground(GRAY);
    
    DrawLeftPanel();
    DrawMiddlePanel();
    DrawRightPanel();
    
    if (requestOverlayActive) {
        DrawRequestOverlay();
    }
}

void SalesFloorScreen::DrawLeftPanel() {
    // Draw panel background
    DrawRectangle(0, 0, leftPanelWidth, screenHeight, DARKBLUE);
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
        DrawText("SELECTED PLANT", 20, yPos, 20, YELLOW);
        yPos += 35;
        
        // Plant name
        std::string plantNameText = selectedPlant->getName();
        DrawText(plantNameText.c_str(), 20, yPos, 18, WHITE);
        yPos += 30;
        
        // Plant ID
        std::string idText = "ID: " + selectedPlant->getID();
        DrawText(idText.c_str(), 20, yPos, 14, LIGHTGRAY);
        yPos += 25;
        
        // Plant state
        std::string stateText = "State: " + selectedPlant->getState()->getStateName();
        DrawText(stateText.c_str(), 20, yPos, 14, LIGHTGRAY);
        yPos += 25;
        
        // Plant age
        std::ostringstream ageStream;
        ageStream << "Age: " << selectedPlant->getAge() << " days";
        DrawText(ageStream.str().c_str(), 20, yPos, 14, LIGHTGRAY);
        yPos += 25;
        
        // Health level
        std::ostringstream healthStream;
        healthStream << "Health: " << selectedPlant->getHealthLevel() << "%";
        Color healthColor = selectedPlant->getHealthLevel() > 70 ? GREEN : 
                           selectedPlant->getHealthLevel() > 40 ? YELLOW : RED;
        DrawText(healthStream.str().c_str(), 20, yPos, 14, healthColor);
        yPos += 25;
        
        // Water level
        std::ostringstream waterStream;
        waterStream << "Water: " << selectedPlant->getWaterLevel() << "%";
        DrawText(waterStream.str().c_str(), 20, yPos, 14, SKYBLUE);
        yPos += 25;
        
        // Nutrient level
        std::ostringstream nutrientStream;
        nutrientStream << "Nutrients: " << selectedPlant->getNutrientLevel() << "%";
        DrawText(nutrientStream.str().c_str(), 20, yPos, 14, Color{150, 255, 150, 255});
        yPos += 30;
        
        // Price (larger and highlighted)
        std::ostringstream priceStream;
        priceStream << "Price: R" << std::fixed << std::setprecision(2) << selectedPlant->getPrice();
        DrawText(priceStream.str().c_str(), 20, yPos, 20, GOLD);
        yPos += 40;
        
    } else {
        // No plant selected - show instructions
        DrawText("INSTRUCTIONS:", 20, yPos, 18, YELLOW);
        yPos += 30;
        
        DrawText("- Click plants to select", 20, yPos, 14, WHITE);
        yPos += 25;
        DrawText("- View plant details here", 20, yPos, 14, WHITE);
        yPos += 25;
        DrawText("- Add to cart when ready", 20, yPos, 14, WHITE);
        yPos += 25;
        DrawText("- Use buttons to navigate", 20, yPos, 14, WHITE);
    }
}

void SalesFloorScreen::DrawMiddlePanel() {
    // Draw panel background
    DrawRectangle(leftPanelWidth, 0, middlePanelWidth, screenHeight, Color{100, 100, 100, 255});
    DrawLine(leftPanelWidth + middlePanelWidth, 0, leftPanelWidth + middlePanelWidth, screenHeight, BLACK);
    
    // Draw header
    const char* header = "SALES FLOOR";
    int headerSize = 24;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header, 
             leftPanelWidth + (middlePanelWidth - headerWidth) / 2, 
             20, 
             headerSize, 
             BLACK);
    
    // Draw grid
    DrawGrid();
}

void SalesFloorScreen::DrawRightPanel() {
    // Draw panel background
    DrawRectangle(leftPanelWidth + middlePanelWidth, 0, rightPanelWidth, screenHeight, DARKGREEN);
    
    // Draw header
    const char* header = "CART";
    int headerSize = 20;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header, 
             screenWidth - rightPanelWidth + (rightPanelWidth - headerWidth) / 2, 
             20, 
             headerSize, 
             WHITE);
    
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;
    
    // Draw cart items with remove buttons
    std::vector<Plant*> cart = customer->getCart();
    int yPos = 60;
    
    if (cart.empty()) {
        DrawText("Cart is empty", screenWidth - rightPanelWidth + 20, yPos, 16, LIGHTGRAY);
    } else {
        Vector2 mousePos = GetMousePosition();
        
        for (size_t i = 0; i < cart.size(); i++) {
            Plant* plant = cart[i];
            if (plant != nullptr) {
                // Draw remove button (X)
                int removeX = screenWidth - rightPanelWidth + 20;
                int removeY = yPos;
                Rectangle removeBtn = Rectangle{
                    static_cast<float>(removeX),
                    static_cast<float>(removeY),
                    20,
                    20
                };
                
                bool removeHovered = CheckCollisionPointRec(mousePos, removeBtn);
                Color removeBtnColor = removeHovered ? RED : Color{150, 50, 50, 255};
                DrawRectangleRec(removeBtn, removeBtnColor);
                DrawRectangleLinesEx(removeBtn, 1, BLACK);
                DrawText("X", removeX + 6, removeY + 3, 14, WHITE);
                
                // Handle remove click
                if (removeHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    std::cout << "[SalesFloorScreen] Removing item " << i << " from cart" << std::endl;
                    customer->returnPlantToSalesFloor(i);
                    break; // Exit loop since cart changed
                }
                
                // Draw plant name (indented)
                std::string itemText = plant->getName();
                DrawText(itemText.c_str(), screenWidth - rightPanelWidth + 45, yPos + 3, 14, WHITE);
                yPos += 25;
                
                // Draw price
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
        
        // Calculate total from cart
        double cartTotal = 0.0;
        for (Plant* p : cart) {
            if (p != nullptr) {
                cartTotal += p->getPrice();
            }
        }
        
        std::ostringstream totalStream;
        totalStream << "Total: R" << std::fixed << std::setprecision(2) << cartTotal;
        DrawText(totalStream.str().c_str(), screenWidth - rightPanelWidth + 20, yPos, 16, GREEN);
    }
    
    // Draw buttons
    DrawButtons();
}

void SalesFloorScreen::DrawGrid() {
    SalesFloor* salesFloor = manager->GetSalesFloor();
    if (salesFloor == nullptr) return;
    
    for (int row = 0; row < gridRows; row++) {
        for (int col = 0; col < gridCols; col++) {
            int x = gridStartX + col * cellSize;
            int y = gridStartY + row * cellSize;
            
            // Determine cell color
            Color cellColor = LIGHTGRAY;
            if (row == selectedRow && col == selectedCol) {
                cellColor = SKYBLUE; // Selected
            }
            
            // Draw cell background
            DrawRectangle(x + 2, y + 2, cellSize - 4, cellSize - 4, cellColor);
            DrawRectangleLines(x + 2, y + 2, cellSize - 4, cellSize - 4, BLACK);
            
            // Draw plant if exists
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
    
    // Get plant texture
    Texture2D plantTexture = manager->GetPlantTexture(plant->getName());
    
    if (plantTexture.id != 0) {
        // Calculate scaling to fit in cell (leave padding)
        int maxSize = cellSize - 30;
        float scale = static_cast<float>(maxSize) / plantTexture.height;
        
        int scaledWidth = static_cast<int>(plantTexture.width * scale);
        int scaledHeight = static_cast<int>(plantTexture.height * scale);
        
        // Center texture in cell
        int texX = x + (cellSize - scaledWidth) / 2;
        int texY = y + (cellSize - scaledHeight) / 2 - 10;
        
        DrawTextureEx(plantTexture, Vector2{static_cast<float>(texX), static_cast<float>(texY)}, 0.0f, scale, WHITE);
    } else {
        // Fallback: draw plant name if no texture
        const char* name = plant->getName().c_str();
        int nameWidth = MeasureText(name, 12);
        DrawText(name, x + (cellSize - nameWidth) / 2, y + cellSize / 2 - 10, 12, DARKGREEN);
    }
    
    // Draw price
    std::ostringstream priceStream;
    priceStream << "R" << static_cast<int>(plant->getPrice());
    std::string priceText = priceStream.str();
    int priceWidth = MeasureText(priceText.c_str(), 14);
    DrawText(priceText.c_str(), x + (cellSize - priceWidth) / 2, y + cellSize - 25, 14, DARKGREEN);
}

void SalesFloorScreen::DrawButtons() {
    Customer* customer = manager->GetCustomer();
    bool hasCartItems = (customer != nullptr && customer->getCartSize() > 0);
    
    // Add to Cart button
    Color addColor = (selectedPlant != nullptr) ? 
                     (addToCartHovered ? DARKGREEN : GREEN) : GRAY;
    DrawRectangleRec(addToCartButton, addColor);
    DrawRectangleLinesEx(addToCartButton, 2, BLACK);
    const char* addText = "Add to Cart";
    int addTextWidth = MeasureText(addText, 18);
    DrawText(addText,
             addToCartButton.x + (addToCartButton.width - addTextWidth) / 2,
             addToCartButton.y + (addToCartButton.height - 18) / 2,
             18,
             WHITE);
    
    // View Greenhouse button
    Color greenhouseColor = viewGreenhouseHovered ? DARKBLUE : Color{50, 80, 120, 255};
    DrawRectangleRec(viewGreenhouseButton, greenhouseColor);
    DrawRectangleLinesEx(viewGreenhouseButton, 2, BLACK);
    const char* greenhouseText = "View Greenhouse";
    int greenhouseTextWidth = MeasureText(greenhouseText, 16);
    DrawText(greenhouseText,
             viewGreenhouseButton.x + (viewGreenhouseButton.width - greenhouseTextWidth) / 2,
             viewGreenhouseButton.y + (viewGreenhouseButton.height - 16) / 2,
             16,
             WHITE);
    
    // View Cart button
    Color cartColor = hasCartItems ? 
                      (viewCartHovered ? ORANGE : Color{200, 120, 50, 255}) : GRAY;
    DrawRectangleRec(viewCartButton, cartColor);
    DrawRectangleLinesEx(viewCartButton, 2, BLACK);
    const char* cartText = "View Cart";
    int cartTextWidth = MeasureText(cartText, 18);
    DrawText(cartText,
             viewCartButton.x + (viewCartButton.width - cartTextWidth) / 2,
             viewCartButton.y + (viewCartButton.height - 18) / 2,
             18,
             WHITE);
    
    // Make Request button
    Color requestColor = makeRequestHovered ? Color{150, 50, 150, 255} : Color{100, 50, 100, 255};
    DrawRectangleRec(makeRequestButton, requestColor);
    DrawRectangleLinesEx(makeRequestButton, 2, BLACK);
    const char* requestText = "Make Request";
    int requestTextWidth = MeasureText(requestText, 16);
    DrawText(requestText,
             makeRequestButton.x + (makeRequestButton.width - requestTextWidth) / 2,
             makeRequestButton.y + (makeRequestButton.height - 16) / 2,
             16,
             WHITE);
}

void SalesFloorScreen::DrawRequestOverlay() {
    // Dim background
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});
    
    // Draw overlay box
    int overlayWidth = 600;
    int overlayHeight = 400;
    int overlayX = (screenWidth - overlayWidth) / 2;
    int overlayY = (screenHeight - overlayHeight) / 2;
    
    DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, Color{40, 40, 60, 255});
    DrawRectangleLinesEx(Rectangle{static_cast<float>(overlayX), static_cast<float>(overlayY), 
                                   static_cast<float>(overlayWidth), static_cast<float>(overlayHeight)}, 
                        3, GOLD);
    
    // Title
    const char* title = "Submit Request to Staff";
    int titleWidth = MeasureText(title, 24);
    DrawText(title, overlayX + (overlayWidth - titleWidth) / 2, overlayY + 20, 24, WHITE);
    
    // Text input area
    int inputY = overlayY + 70;
    DrawRectangle(overlayX + 20, inputY, overlayWidth - 40, 40, WHITE);
    DrawRectangleLines(overlayX + 20, inputY, overlayWidth - 40, 40, BLACK);
    DrawText(requestText, overlayX + 30, inputY + 10, 20, BLACK);
    
    // Blinking cursor
    if (static_cast<int>(GetTime() * 2) % 2 == 0) {
        int cursorX = overlayX + 30 + MeasureText(requestText, 20);
        DrawText("|", cursorX, inputY + 10, 20, BLACK);
    }
    
    // Instructions
    DrawText("Type your request and press ENTER to submit", 
             overlayX + 20, inputY + 50, 16, LIGHTGRAY);
    DrawText("Press ESC to cancel", 
             overlayX + 20, inputY + 75, 16, LIGHTGRAY);
    
    // Response area
    if (!responseText.empty()) {
        int responseY = inputY + 110;
        DrawText("Response:", overlayX + 20, responseY, 18, YELLOW);
        
        // Draw response text (word wrap)
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
    
    // Close button
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