#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

enum GameState {
    SHOPPING,
    CUSTOMIZATION,
    CHECKOUT
};

struct SpriteLayer {
    Texture2D texture;
    Vector2 offset;
    Color tint;
    float price;
    std::string name;
};

struct Plant {
    int row;
    int col;
    float basePrice;
    bool inStock;
    int textureIndex;  // Which plant texture to use (0=rose, 1=radish, 2=cactus)
    std::vector<SpriteLayer> customizations;
    
    float GetTotalPrice() {
        float total = basePrice;
        for (const auto& layer : customizations) {
            total += layer.price;
        }
        return total;
    }
};

int main() {
    srand(time(NULL));
    
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Plant Nursery Shop");
    SetTargetFPS(60);
    
    // Load base plant textures (displayed on sales floor)
    Texture2D plantTextures[3];
    plantTextures[0] = LoadTexture("assets/rose.png");
    plantTextures[1] = LoadTexture("assets/radish.png");
    plantTextures[2] = LoadTexture("assets/cactus.png");
    
    // Load customization textures
    Texture2D potTextures[3];
    potTextures[0] = LoadTexture("assets/pot_standard.png");
    potTextures[1] = LoadTexture("assets/pot_red.png");
    potTextures[2] = LoadTexture("assets/pot_terracotta.png");
    
    // Calculate section widths
    const int leftWidth = screenWidth * 0.25f;
    const int middleWidth = screenWidth * 0.50f;
    const int rightWidth = screenWidth * 0.25f;
    
    // Grid settings
    const int gridSize = 5;
    const int cellSize = (middleWidth < screenHeight ? middleWidth : screenHeight) / gridSize - 10;
    const int gridStartX = leftWidth + (middleWidth - cellSize * gridSize) / 2;
    const int gridStartY = (screenHeight - cellSize * gridSize) / 2;
    
    // Initialize plants with random prices and textures
    Plant plants[5][5];
    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            plants[row][col].row = row;
            plants[row][col].col = col;
            plants[row][col].basePrice = 5.0f + (rand() % 46); // Random price $5-$50
            plants[row][col].inStock = true;
            plants[row][col].textureIndex = rand() % 3;  // Randomly assign 0, 1, or 2
        }
    }
    
    // Player data
    float playerMoney = 500.0f;
    std::vector<Plant*> cart;
    
    // Game state
    GameState state = SHOPPING;
    int selectedPayment = 0; // 0=Cash, 1=Card, 2=Mobile
    
    // Track hovered cell
    int hoveredRow = -1;
    int hoveredCol = -1;
    
    // Customization state
    int currentCustomizingIndex = 0;
    
    // Checkout button
    Rectangle checkoutBtn = {(float)leftWidth + middleWidth + 20, (float)screenHeight - 80, 
                              (float)rightWidth - 40, 50};
    
    // Customization buttons - CENTERED
    const int potBtnSize = 120;
    const int potBtnSpacing = 40;
    const int totalPotWidth = (potBtnSize * 3) + (potBtnSpacing * 2);
    const int potBtnStartX = (screenWidth - totalPotWidth) / 2;
    const int potBtnY = 450;
    
    Rectangle potBtns[3] = {
        {(float)potBtnStartX, (float)potBtnY, (float)potBtnSize, (float)potBtnSize},
        {(float)(potBtnStartX + potBtnSize + potBtnSpacing), (float)potBtnY, (float)potBtnSize, (float)potBtnSize},
        {(float)(potBtnStartX + (potBtnSize + potBtnSpacing) * 2), (float)potBtnY, (float)potBtnSize, (float)potBtnSize}
    };
    
    Rectangle nextPlantBtn = {(float)screenWidth - 220, (float)screenHeight - 80, 180, 50};
    Rectangle finishCustomBtn = {(float)screenWidth / 2 - 90, (float)screenHeight - 80, 180, 50};
    Rectangle backBtn = {40, 30, 100, 40};
    
    // Payment buttons - CENTERED
    const int payBtnWidth = 150;
    const int payBtnHeight = 70;
    const int payBtnSpacing = 30;
    const int totalPayWidth = (payBtnWidth * 3) + (payBtnSpacing * 2);
    const int payBtnStartX = (screenWidth - totalPayWidth) / 2;
    const int payBtnY = 320;
    
    Rectangle cashBtn = {(float)payBtnStartX, (float)payBtnY, (float)payBtnWidth, (float)payBtnHeight};
    Rectangle cardBtn = {(float)(payBtnStartX + payBtnWidth + payBtnSpacing), (float)payBtnY, (float)payBtnWidth, (float)payBtnHeight};
    Rectangle mobileBtn = {(float)(payBtnStartX + (payBtnWidth + payBtnSpacing) * 2), (float)payBtnY, (float)payBtnWidth, (float)payBtnHeight};
    Rectangle confirmBtn = {(float)screenWidth / 2 - 100, 480, 200, 60};
    
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        
        if (state == SHOPPING) {
            // Check if mouse is over grid
            hoveredRow = -1;
            hoveredCol = -1;
            if (mousePos.x >= gridStartX && mousePos.x < gridStartX + cellSize * gridSize &&
                mousePos.y >= gridStartY && mousePos.y < gridStartY + cellSize * gridSize) {
                hoveredCol = (mousePos.x - gridStartX) / cellSize;
                hoveredRow = (mousePos.y - gridStartY) / cellSize;
                
                // Click to add/remove from cart
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && plants[hoveredRow][hoveredCol].inStock) {
                    Plant* p = &plants[hoveredRow][hoveredCol];
                    
                    // Check if already in cart
                    bool found = false;
                    for (size_t i = 0; i < cart.size(); i++) {
                        if (cart[i] == p) {
                            cart.erase(cart.begin() + i);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cart.push_back(p);
                    }
                }
            }
            
            // Check checkout button
            if (CheckCollisionPointRec(mousePos, checkoutBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (cart.size() > 0) {
                    state = CUSTOMIZATION;
                    currentCustomizingIndex = 0;
                }
            }
        }
        else if (state == CUSTOMIZATION) {
            Plant* currentPlant = cart[currentCustomizingIndex];
            
            // Check pot selection buttons
            for (int i = 0; i < 3; i++) {
                if (CheckCollisionPointRec(mousePos, potBtns[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    // Remove existing pot if any
                    for (size_t j = 0; j < currentPlant->customizations.size(); j++) {
                        if (currentPlant->customizations[j].name.find("Pot") != std::string::npos) {
                            currentPlant->customizations.erase(currentPlant->customizations.begin() + j);
                            break;
                        }
                    }
                    
                    // Add new pot
                    SpriteLayer pot;
                    pot.texture = potTextures[i];
                    pot.offset = {0, 0};
                    pot.tint = WHITE;
                    pot.price = 3.0f + i * 2.0f;
                    if (i == 0) pot.name = "Blue Pot";
                    else if (i == 1) pot.name = "Red Pot";
                    else pot.name = "Terracotta Pot";
                    
                    currentPlant->customizations.push_back(pot);
                }
            }
            
            // Next plant button
            if (CheckCollisionPointRec(mousePos, nextPlantBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (currentCustomizingIndex < (int)cart.size() - 1) {
                    currentCustomizingIndex++;
                }
            }
            
            // Finish customization button
            if (CheckCollisionPointRec(mousePos, finishCustomBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                state = CHECKOUT;
            }
            
            // Back button
            if (CheckCollisionPointRec(mousePos, backBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                // Clear all customizations
                for (Plant* p : cart) {
                    p->customizations.clear();
                }
                state = SHOPPING;
            }
        }
        else if (state == CHECKOUT) {
            // Check payment method selection
            if (CheckCollisionPointRec(mousePos, cashBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedPayment = 0;
            }
            if (CheckCollisionPointRec(mousePos, cardBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedPayment = 1;
            }
            if (CheckCollisionPointRec(mousePos, mobileBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedPayment = 2;
            }
            
            // Check back button
            if (CheckCollisionPointRec(mousePos, backBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                state = CUSTOMIZATION;
            }
            
            // Confirm purchase
            if (CheckCollisionPointRec(mousePos, confirmBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                float total = 0;
                for (Plant* p : cart) {
                    total += p->GetTotalPrice();
                }
                
                if (playerMoney >= total) {
                    playerMoney -= total;
                    // Remove plants from stock
                    for (Plant* p : cart) {
                        p->inStock = false;
                        p->customizations.clear();
                    }
                    cart.clear();
                    state = SHOPPING;
                }
            }
        }
        
        BeginDrawing();
        ClearBackground(DARKGRAY);
        
        if (state == SHOPPING) {
            // Left section - Player info
            DrawRectangle(0, 0, leftWidth, screenHeight, DARKBLUE);
            DrawText("CUSTOMER INFO", 20, 20, 18, WHITE);
            DrawText(TextFormat("Money: $%.2f", playerMoney), 20, 60, 16, GREEN);
            DrawText("", 20, 100, 14, LIGHTGRAY);
            DrawText("Click plants to", 20, 120, 14, LIGHTGRAY);
            DrawText("add to cart!", 20, 140, 14, LIGHTGRAY);
            
            // Middle section - Sales floor grid
            DrawRectangle(leftWidth, 0, middleWidth, screenHeight, GRAY);
            DrawText("SALES FLOOR", leftWidth + 20, 20, 20, BLACK);
            
            // Draw grid
            for (int row = 0; row < gridSize; row++) {
                for (int col = 0; col < gridSize; col++) {
                    int x = gridStartX + col * cellSize;
                    int y = gridStartY + row * cellSize;
                    
                    // Check if in cart
                    bool inCart = false;
                    for (Plant* p : cart) {
                        if (p->row == row && p->col == col) {
                            inCart = true;
                            break;
                        }
                    }
                    
                    Color cellColor = LIGHTGRAY;
                    if (inCart) {
                        cellColor = GOLD;
                    } else if (row == hoveredRow && col == hoveredCol) {
                        cellColor = SKYBLUE;
                    }
                    
                    DrawRectangle(x, y, cellSize - 2, cellSize - 2, cellColor);
                    DrawRectangleLines(x, y, cellSize - 2, cellSize - 2, BLACK);
                    
                    // Only draw plant texture and price if in stock
                    if (plants[row][col].inStock) {
                        // Get the correct texture for this plant
                        Texture2D currentTexture = plantTextures[plants[row][col].textureIndex];
                        
                        // Draw base plant texture - CENTERED
                        float scale = (cellSize - 30) / (float)currentTexture.width;
                        if (currentTexture.height * scale > cellSize - 30) {
                            scale = (cellSize - 30) / (float)currentTexture.height;
                        }
                        
                        int texWidth = currentTexture.width * scale;
                        int texHeight = currentTexture.height * scale;
                        int texX = x + (cellSize - texWidth) / 2;
                        int texY = y + (cellSize - texHeight) / 2 - 10;
                        
                        DrawTextureEx(currentTexture, (Vector2){(float)texX, (float)texY}, 0.0f, scale, WHITE);
                        
                        // Draw price - CENTERED
                        const char* priceText = TextFormat("$%.0f", plants[row][col].basePrice);
                        int textWidth = MeasureText(priceText, 12);
                        DrawText(priceText, x + (cellSize - textWidth) / 2, y + cellSize - 20, 12, DARKGREEN);
                    } else {
                        // Draw "SOLD" text for purchased plants - CENTERED
                        int soldWidth = MeasureText("SOLD", 14);
                        DrawText("SOLD", x + (cellSize - soldWidth) / 2, y + cellSize / 2 - 7, 14, RED);
                    }
                }
            }
            
            // Right section - Cart
            DrawRectangle(leftWidth + middleWidth, 0, rightWidth, screenHeight, DARKGREEN);
            DrawText("SHOPPING CART", leftWidth + middleWidth + 20, 20, 18, WHITE);
            
            float cartTotal = 0;
            int yPos = 60;
            for (size_t i = 0; i < cart.size(); i++) {
                DrawText(TextFormat("Plant #%d", i + 1), leftWidth + middleWidth + 20, yPos, 14, BEIGE);
                DrawText(TextFormat("$%.0f", cart[i]->basePrice), leftWidth + middleWidth + 20, yPos + 20, 14, YELLOW);
                cartTotal += cart[i]->basePrice;
                yPos += 50;
            }
            
            DrawText("-------------", leftWidth + middleWidth + 20, yPos, 14, WHITE);
            DrawText(TextFormat("Total: $%.2f", cartTotal), leftWidth + middleWidth + 20, yPos + 20, 16, GOLD);
            DrawText("(+ customizations)", leftWidth + middleWidth + 20, yPos + 40, 12, LIGHTGRAY);
            
            // Checkout button
            Color btnColor = cart.size() > 0 ? GREEN : GRAY;
            DrawRectangleRec(checkoutBtn, btnColor);
            DrawRectangleLinesEx(checkoutBtn, 2, BLACK);
            DrawText("CHECKOUT", checkoutBtn.x + 35, checkoutBtn.y + 15, 20, WHITE);
        }
        else if (state == CUSTOMIZATION) {
            // Customization screen
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){30, 50, 40, 255});
            
            // Title - CENTERED
            const char* title = "CUSTOMIZE YOUR PLANTS";
            int titleWidth = MeasureText(title, 30);
            DrawText(title, screenWidth / 2 - titleWidth / 2, 30, 30, WHITE);
            
            Plant* currentPlant = cart[currentCustomizingIndex];
            
            // Get the current plant's texture based on its textureIndex
            Texture2D currentPlantTexture = plantTextures[currentPlant->textureIndex];
            
            // Show which plant we're customizing - CENTERED
            const char* plantNum = TextFormat("Plant %d of %d", currentCustomizingIndex + 1, (int)cart.size());
            int plantNumWidth = MeasureText(plantNum, 20);
            DrawText(plantNum, screenWidth / 2 - plantNumWidth / 2, 80, 20, YELLOW);
            
            // Preview area - CENTERED
            Rectangle previewBox = {(float)screenWidth / 2 - 150, 120, 300, 280};
            DrawRectangleRec(previewBox, (Color){50, 70, 60, 255});
            DrawRectangleLinesEx(previewBox, 3, GOLD);
            
            // Calculate centered position for plant preview
            float previewScale = 210.0f / currentPlantTexture.height;
            int scaledWidth = currentPlantTexture.width * previewScale;
            int scaledHeight = currentPlantTexture.height * previewScale;
            
            Vector2 plantPreviewPos = {
                previewBox.x + (previewBox.width - scaledWidth) / 2,
                previewBox.y + (previewBox.height - scaledHeight) / 2 - 20
            };
            
            // Draw plant FIRST (behind pot)
            DrawTextureEx(currentPlantTexture, plantPreviewPos, 0.0f, previewScale, WHITE);
            
            // Draw pot in FRONT of plant
            for (const auto& layer : currentPlant->customizations) {
                // Calculate pot scale and position to fit nicely with plant
                float potScale = 220.0f / layer.texture.width;  // Slightly bigger
                int potWidth = layer.texture.width * potScale;
                int potHeight = layer.texture.height * potScale;
                Vector2 potPos = {
                    previewBox.x + (previewBox.width - potWidth) / 2,
                    plantPreviewPos.y + scaledHeight - potHeight + 30  // Position pot so plant stem goes into it
                };
                DrawTextureEx(layer.texture, potPos, 0.0f, potScale, layer.tint);
            }
            
            // Pot selection label - CENTERED
            const char* selectPot = "Select Pot:";
            int selectPotWidth = MeasureText(selectPot, 22);
            DrawText(selectPot, screenWidth / 2 - selectPotWidth / 2, 420, 22, WHITE);
            
            // Draw pot selection buttons
            for (int i = 0; i < 3; i++) {
                Color btnColor = LIGHTGRAY;
                // Check if this pot is selected
                for (const auto& layer : currentPlant->customizations) {
                    if ((i == 0 && layer.name == "Blue Pot") ||
                        (i == 1 && layer.name == "Red Pot") ||
                        (i == 2 && layer.name == "Terracotta Pot")) {
                        btnColor = SKYBLUE;
                        break;
                    }
                }
                
                DrawRectangleRec(potBtns[i], btnColor);
                DrawRectangleLinesEx(potBtns[i], 3, BLACK);
                
                // Draw pot preview - CENTERED in button
                if (potTextures[i].id != 0) {  // Check if texture loaded
                    float btnScale = 90.0f / potTextures[i].width;
                    int potW = potTextures[i].width * btnScale;
                    int potH = potTextures[i].height * btnScale;
                    Vector2 potBtnPos = {
                        potBtns[i].x + (potBtns[i].width - potW) / 2,
                        potBtns[i].y + (potBtns[i].height - potH) / 2 - 10
                    };
                    DrawTextureEx(potTextures[i], potBtnPos, 0.0f, btnScale, WHITE);
                }
                
                // Draw price - CENTERED
                const char* prices[] = {"$3", "$5", "$7"};
                int priceWidth = MeasureText(prices[i], 18);
                DrawText(prices[i], potBtns[i].x + (potBtns[i].width - priceWidth) / 2, 
                        potBtns[i].y + potBtns[i].height + 8, 18, DARKGREEN);
            }
            
            // Show pricing - CENTERED
            float customCost = 0;
            for (const auto& layer : currentPlant->customizations) {
                customCost += layer.price;
            }
            
            const char* basePrice = TextFormat("Base Price: $%.2f", currentPlant->basePrice);
            const char* customPrice = TextFormat("Customizations: +$%.2f", customCost);
            const char* totalPrice = TextFormat("Total: $%.2f", currentPlant->GetTotalPrice());
            
            int basePriceW = MeasureText(basePrice, 18);
            int customPriceW = MeasureText(customPrice, 18);
            int totalPriceW = MeasureText(totalPrice, 22);
            
            DrawText(basePrice, screenWidth / 2 - basePriceW / 2, 620, 18, WHITE);
            DrawText(customPrice, screenWidth / 2 - customPriceW / 2, 645, 18, YELLOW);
            DrawText(totalPrice, screenWidth / 2 - totalPriceW / 2, 675, 22, GOLD);
            
            // Navigation buttons
            if (currentCustomizingIndex < (int)cart.size() - 1) {
                DrawRectangleRec(nextPlantBtn, BLUE);
                DrawRectangleLinesEx(nextPlantBtn, 2, BLACK);
                int nextWidth = MeasureText("NEXT PLANT", 18);
                DrawText("NEXT PLANT", nextPlantBtn.x + (nextPlantBtn.width - nextWidth) / 2, 
                        nextPlantBtn.y + 15, 18, WHITE);
            }
            
            DrawRectangleRec(finishCustomBtn, GREEN);
            DrawRectangleLinesEx(finishCustomBtn, 2, BLACK);
            int finishWidth = MeasureText("FINISH", 18);
            DrawText("FINISH", finishCustomBtn.x + (finishCustomBtn.width - finishWidth) / 2, 
                    finishCustomBtn.y + 15, 18, WHITE);
            
            // Back button
            DrawRectangleRec(backBtn, ORANGE);
            DrawRectangleLinesEx(backBtn, 2, BLACK);
            int backWidth = MeasureText("BACK", 16);
            DrawText("BACK", backBtn.x + (backBtn.width - backWidth) / 2, backBtn.y + 12, 16, WHITE);
        }
        else if (state == CHECKOUT) {
            // Checkout screen
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){40, 40, 60, 255});
            
            // Title - CENTERED
            const char* checkoutTitle = "CHECKOUT";
            int checkoutTitleW = MeasureText(checkoutTitle, 40);
            DrawText(checkoutTitle, screenWidth / 2 - checkoutTitleW / 2, 80, 40, WHITE);
            
            float total = 0;
            for (Plant* p : cart) {
                total += p->GetTotalPrice();
            }
            
            // Total and balance - CENTERED
            const char* totalText = TextFormat("Total: $%.2f", total);
            const char* balanceText = TextFormat("Your Balance: $%.2f", playerMoney);
            int totalW = MeasureText(totalText, 32);
            int balanceW = MeasureText(balanceText, 22);
            
            DrawText(totalText, screenWidth / 2 - totalW / 2, 160, 32, GOLD);
            DrawText(balanceText, screenWidth / 2 - balanceW / 2, 210, 22, GREEN);
            
            if (playerMoney < total) {
                const char* insufficientText = "INSUFFICIENT FUNDS!";
                int insufficientW = MeasureText(insufficientText, 26);
                DrawText(insufficientText, screenWidth / 2 - insufficientW / 2, 250, 26, RED);
            }
            
            // Payment method label - CENTERED
            const char* paymentLabel = "Select Payment Method:";
            int paymentLabelW = MeasureText(paymentLabel, 22);
            DrawText(paymentLabel, screenWidth / 2 - paymentLabelW / 2, 280, 22, WHITE);
            
            // Payment method buttons
            Color cashColor = selectedPayment == 0 ? SKYBLUE : LIGHTGRAY;
            Color cardColor = selectedPayment == 1 ? SKYBLUE : LIGHTGRAY;
            Color mobileColor = selectedPayment == 2 ? SKYBLUE : LIGHTGRAY;
            
            DrawRectangleRec(cashBtn, cashColor);
            DrawRectangleLinesEx(cashBtn, 3, BLACK);
            int cashW = MeasureText("CASH", 22);
            DrawText("CASH", cashBtn.x + (cashBtn.width - cashW) / 2, cashBtn.y + 24, 22, BLACK);
            
            DrawRectangleRec(cardBtn, cardColor);
            DrawRectangleLinesEx(cardBtn, 3, BLACK);
            int cardW = MeasureText("CARD", 22);
            DrawText("CARD", cardBtn.x + (cardBtn.width - cardW) / 2, cardBtn.y + 24, 22, BLACK);
            
            DrawRectangleRec(mobileBtn, mobileColor);
            DrawRectangleLinesEx(mobileBtn, 3, BLACK);
            int mobileW = MeasureText("MOBILE", 22);
            DrawText("MOBILE", mobileBtn.x + (mobileBtn.width - mobileW) / 2, mobileBtn.y + 24, 22, BLACK);
            
            // Confirm button
            Color confirmColor = playerMoney >= total ? GREEN : GRAY;
            DrawRectangleRec(confirmBtn, confirmColor);
            DrawRectangleLinesEx(confirmBtn, 3, BLACK);
            int confirmW = MeasureText("CONFIRM", 26);
            DrawText("CONFIRM", confirmBtn.x + (confirmBtn.width - confirmW) / 2, confirmBtn.y + 17, 26, WHITE);
            
            // Back button
            DrawRectangleRec(backBtn, ORANGE);
            DrawRectangleLinesEx(backBtn, 2, BLACK);
            int backW = MeasureText("BACK", 16);
            DrawText("BACK", backBtn.x + (backBtn.width - backW) / 2, backBtn.y + 12, 16, WHITE);
        }
        
        // Draw section dividers (shopping mode only)
        if (state == SHOPPING) {
            DrawLine(leftWidth, 0, leftWidth, screenHeight, BLACK);
            DrawLine(leftWidth + middleWidth, 0, leftWidth + middleWidth, screenHeight, BLACK);
        }
        
        EndDrawing();
    }
    
    // Unload all textures
    for (int i = 0; i < 3; i++) {
        UnloadTexture(plantTextures[i]);
        UnloadTexture(potTextures[i]);
    }
    
    CloseWindow();
    return 0;
}