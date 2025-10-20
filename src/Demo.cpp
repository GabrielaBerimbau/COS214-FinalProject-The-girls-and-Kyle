#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstdio>

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
    int textureIndex;
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
    
    // CRITICAL: Set to lower FPS target to reduce WSL overhead
    SetTargetFPS(30);
    
    // Load base plant textures
    Texture2D plantTextures[3];
    plantTextures[0] = LoadTexture("assets/carrot.png");
    plantTextures[1] = LoadTexture("assets/strelitzia.png");
    plantTextures[2] = LoadTexture("assets/cactus2.png");
    
    // CRITICAL: Use point filtering (faster than bilinear)
    for (int i = 0; i < 3; i++) {
        SetTextureFilter(plantTextures[i], TEXTURE_FILTER_POINT);
    }
    
    // Load customization textures
    Texture2D potTextures[3];
    potTextures[0] = LoadTexture("assets/pot_standard.png");
    potTextures[1] = LoadTexture("assets/pot_red.png");
    potTextures[2] = LoadTexture("assets/pot_terracotta.png");
    
    for (int i = 0; i < 3; i++) {
        SetTextureFilter(potTextures[i], TEXTURE_FILTER_POINT);
    }
    
    // Calculate section widths
    const int leftWidth = screenWidth * 0.25f;
    const int middleWidth = screenWidth * 0.50f;
    const int rightWidth = screenWidth * 0.25f;
    
    // Grid settings
    const int gridSize = 5;
    const int cellSize = (middleWidth < screenHeight ? middleWidth : screenHeight) / gridSize - 10;
    const int gridStartX = leftWidth + (middleWidth - cellSize * gridSize) / 2;
    const int gridStartY = (screenHeight - cellSize * gridSize) / 2;
    
    // Initialize plants
    Plant plants[5][5];
    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            plants[row][col].row = row;
            plants[row][col].col = col;
            plants[row][col].basePrice = 5.0f + (rand() % 46);
            plants[row][col].inStock = true;
            plants[row][col].textureIndex = rand() % 3;
        }
    }
    
    // Player data
    float playerMoney = 500.0f;
    std::vector<Plant*> cart;
    
    // Game state
    GameState state = SHOPPING;
    int selectedPayment = 0;
    
    // Track hovered cell
    int hoveredRow = -1;
    int hoveredCol = -1;
    
    // Customization state
    int currentCustomizingIndex = 0;
    
    // Checkout button
    Rectangle checkoutBtn = {(float)leftWidth + middleWidth + 20, (float)screenHeight - 80, 
                              (float)rightWidth - 40, 50};
    
    // Customization buttons
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
    
    // Payment buttons
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
    
    // Pre-allocated text buffers
    char textBuffer[256];
    
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        
        if (state == SHOPPING) {
            hoveredRow = -1;
            hoveredCol = -1;
            if (mousePos.x >= gridStartX && mousePos.x < gridStartX + cellSize * gridSize &&
                mousePos.y >= gridStartY && mousePos.y < gridStartY + cellSize * gridSize) {
                hoveredCol = (mousePos.x - gridStartX) / cellSize;
                hoveredRow = (mousePos.y - gridStartY) / cellSize;
                
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && plants[hoveredRow][hoveredCol].inStock) {
                    Plant* p = &plants[hoveredRow][hoveredCol];
                    
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
            
            if (CheckCollisionPointRec(mousePos, checkoutBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (cart.size() > 0) {
                    state = CUSTOMIZATION;
                    currentCustomizingIndex = 0;
                }
            }
        }
        else if (state == CUSTOMIZATION) {
            Plant* currentPlant = cart[currentCustomizingIndex];
            
            for (int i = 0; i < 3; i++) {
                if (CheckCollisionPointRec(mousePos, potBtns[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    for (size_t j = 0; j < currentPlant->customizations.size(); j++) {
                        if (currentPlant->customizations[j].name.find("Pot") != std::string::npos) {
                            currentPlant->customizations.erase(currentPlant->customizations.begin() + j);
                            break;
                        }
                    }
                    
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
            
            if (CheckCollisionPointRec(mousePos, nextPlantBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (currentCustomizingIndex < (int)cart.size() - 1) {
                    currentCustomizingIndex++;
                }
            }
            
            if (CheckCollisionPointRec(mousePos, finishCustomBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                state = CHECKOUT;
            }
            
            if (CheckCollisionPointRec(mousePos, backBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                for (Plant* p : cart) {
                    p->customizations.clear();
                }
                state = SHOPPING;
            }
        }
        else if (state == CHECKOUT) {
            if (CheckCollisionPointRec(mousePos, cashBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedPayment = 0;
            }
            if (CheckCollisionPointRec(mousePos, cardBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedPayment = 1;
            }
            if (CheckCollisionPointRec(mousePos, mobileBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedPayment = 2;
            }
            
            if (CheckCollisionPointRec(mousePos, backBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                state = CUSTOMIZATION;
            }
            
            if (CheckCollisionPointRec(mousePos, confirmBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                float total = 0;
                for (Plant* p : cart) {
                    total += p->GetTotalPrice();
                }
                
                if (playerMoney >= total) {
                    playerMoney -= total;
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
            // Left section
            DrawRectangle(0, 0, leftWidth, screenHeight, DARKBLUE);
            DrawText("CUSTOMER INFO", 20, 20, 18, WHITE);
            snprintf(textBuffer, sizeof(textBuffer), "Money: $%.2f", playerMoney);
            DrawText(textBuffer, 20, 60, 16, GREEN);
            DrawText("Click plants to", 20, 120, 14, LIGHTGRAY);
            DrawText("add to cart!", 20, 140, 14, LIGHTGRAY);
            
            // Middle section
            DrawRectangle(leftWidth, 0, middleWidth, screenHeight, GRAY);
            DrawText("SALES FLOOR", leftWidth + 20, 20, 20, BLACK);
            
            // Draw grid - SIMPLIFIED
            for (int row = 0; row < gridSize; row++) {
                for (int col = 0; col < gridSize; col++) {
                    int x = gridStartX + col * cellSize;
                    int y = gridStartY + row * cellSize;
                    
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
                    
                    if (plants[row][col].inStock) {
                        Texture2D currentTexture = plantTextures[plants[row][col].textureIndex];
                        
                        // Simpler scaling
                        float scale = (cellSize - 30) / (float)currentTexture.height;
                        
                        int texWidth = currentTexture.width * scale;
                        int texHeight = currentTexture.height * scale;
                        int texX = x + (cellSize - texWidth) / 2;
                        int texY = y + (cellSize - texHeight) / 2 - 10;
                        
                        DrawTextureEx(currentTexture, (Vector2){(float)texX, (float)texY}, 0.0f, scale, WHITE);
                        
                        snprintf(textBuffer, sizeof(textBuffer), "$%.0f", plants[row][col].basePrice);
                        int textWidth = MeasureText(textBuffer, 12);
                        DrawText(textBuffer, x + (cellSize - textWidth) / 2, y + cellSize - 20, 12, DARKGREEN);
                    } else {
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
                snprintf(textBuffer, sizeof(textBuffer), "Plant #%zu - $%.0f", i + 1, cart[i]->basePrice);
                DrawText(textBuffer, leftWidth + middleWidth + 20, yPos, 14, BEIGE);
                cartTotal += cart[i]->basePrice;
                yPos += 30;
            }
            
            DrawText("-------------", leftWidth + middleWidth + 20, yPos, 14, WHITE);
            snprintf(textBuffer, sizeof(textBuffer), "Total: $%.2f", cartTotal);
            DrawText(textBuffer, leftWidth + middleWidth + 20, yPos + 20, 16, GOLD);
            
            Color btnColor = cart.size() > 0 ? GREEN : GRAY;
            DrawRectangleRec(checkoutBtn, btnColor);
            DrawRectangleLinesEx(checkoutBtn, 2, BLACK);
            DrawText("CHECKOUT", checkoutBtn.x + 35, checkoutBtn.y + 15, 20, WHITE);
        }
        else if (state == CUSTOMIZATION) {
            Plant* currentPlant = cart[currentCustomizingIndex];
            
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){30, 50, 40, 255});
            
            const char* title = "CUSTOMIZE YOUR PLANTS";
            int titleWidth = MeasureText(title, 30);
            DrawText(title, screenWidth / 2 - titleWidth / 2, 30, 30, WHITE);
            
            Texture2D currentPlantTexture = plantTextures[currentPlant->textureIndex];
            
            snprintf(textBuffer, sizeof(textBuffer), "Plant %d of %zu", currentCustomizingIndex + 1, cart.size());
            int plantNumWidth = MeasureText(textBuffer, 20);
            DrawText(textBuffer, screenWidth / 2 - plantNumWidth / 2, 80, 20, YELLOW);
            
            Rectangle previewBox = {(float)screenWidth / 2 - 150, 120, 300, 280};
            DrawRectangleRec(previewBox, (Color){50, 70, 60, 255});
            DrawRectangleLinesEx(previewBox, 3, GOLD);
            
            float previewScale = 210.0f / currentPlantTexture.height;
            int scaledWidth = currentPlantTexture.width * previewScale;
            int scaledHeight = currentPlantTexture.height * previewScale;
            
            Vector2 plantPreviewPos = {
                previewBox.x + (previewBox.width - scaledWidth) / 2,
                previewBox.y + (previewBox.height - scaledHeight) / 2 - 20
            };
            
            DrawTextureEx(currentPlantTexture, plantPreviewPos, 0.0f, previewScale, WHITE);
            
            for (const auto& layer : currentPlant->customizations) {
                float potScale = 220.0f / layer.texture.width;
                int potWidth = layer.texture.width * potScale;
                int potHeight = layer.texture.height * potScale;
                Vector2 potPos = {
                    previewBox.x + (previewBox.width - potWidth) / 2,
                    plantPreviewPos.y + scaledHeight - potHeight + 30
                };
                DrawTextureEx(layer.texture, potPos, 0.0f, potScale, layer.tint);
            }
            
            const char* selectPot = "Select Pot:";
            int selectPotWidth = MeasureText(selectPot, 22);
            DrawText(selectPot, screenWidth / 2 - selectPotWidth / 2, 420, 22, WHITE);
            
            const char* potPrices[] = {"$3", "$5", "$7"};
            for (int i = 0; i < 3; i++) {
                Color btnColor = LIGHTGRAY;
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
                
                if (potTextures[i].id != 0) {
                    float btnScale = 90.0f / potTextures[i].width;
                    int potW = potTextures[i].width * btnScale;
                    int potH = potTextures[i].height * btnScale;
                    Vector2 potBtnPos = {
                        potBtns[i].x + (potBtns[i].width - potW) / 2,
                        potBtns[i].y + (potBtns[i].height - potH) / 2 - 10
                    };
                    DrawTextureEx(potTextures[i], potBtnPos, 0.0f, btnScale, WHITE);
                }
                
                int priceWidth = MeasureText(potPrices[i], 18);
                DrawText(potPrices[i], potBtns[i].x + (potBtns[i].width - priceWidth) / 2, 
                        potBtns[i].y + potBtns[i].height + 8, 18, DARKGREEN);
            }
            
            float customCost = 0;
            for (const auto& layer : currentPlant->customizations) {
                customCost += layer.price;
            }
            
            snprintf(textBuffer, sizeof(textBuffer), "Base: $%.2f | Custom: +$%.2f | Total: $%.2f", 
                    currentPlant->basePrice, customCost, currentPlant->GetTotalPrice());
            int priceW = MeasureText(textBuffer, 18);
            DrawText(textBuffer, screenWidth / 2 - priceW / 2, 640, 18, GOLD);
            
            if (currentCustomizingIndex < (int)cart.size() - 1) {
                DrawRectangleRec(nextPlantBtn, BLUE);
                DrawRectangleLinesEx(nextPlantBtn, 2, BLACK);
                DrawText("NEXT PLANT", nextPlantBtn.x + 30, nextPlantBtn.y + 15, 18, WHITE);
            }
            
            DrawRectangleRec(finishCustomBtn, GREEN);
            DrawRectangleLinesEx(finishCustomBtn, 2, BLACK);
            DrawText("FINISH", finishCustomBtn.x + 50, finishCustomBtn.y + 15, 18, WHITE);
            
            DrawRectangleRec(backBtn, ORANGE);
            DrawRectangleLinesEx(backBtn, 2, BLACK);
            DrawText("BACK", backBtn.x + 30, backBtn.y + 12, 16, WHITE);
        }
        else if (state == CHECKOUT) {
            float total = 0;
            for (Plant* p : cart) {
                total += p->GetTotalPrice();
            }
            
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){40, 40, 60, 255});
            
            const char* checkoutTitle = "CHECKOUT";
            int checkoutTitleW = MeasureText(checkoutTitle, 40);
            DrawText(checkoutTitle, screenWidth / 2 - checkoutTitleW / 2, 80, 40, WHITE);
            
            snprintf(textBuffer, sizeof(textBuffer), "Total: $%.2f", total);
            int totalW = MeasureText(textBuffer, 32);
            DrawText(textBuffer, screenWidth / 2 - totalW / 2, 160, 32, GOLD);
            
            snprintf(textBuffer, sizeof(textBuffer), "Your Balance: $%.2f", playerMoney);
            int balanceW = MeasureText(textBuffer, 22);
            DrawText(textBuffer, screenWidth / 2 - balanceW / 2, 210, 22, GREEN);
            
            if (playerMoney < total) {
                const char* insufficientText = "INSUFFICIENT FUNDS!";
                int insufficientW = MeasureText(insufficientText, 26);
                DrawText(insufficientText, screenWidth / 2 - insufficientW / 2, 250, 26, RED);
            }
            
            const char* paymentLabel = "Select Payment Method:";
            int paymentLabelW = MeasureText(paymentLabel, 22);
            DrawText(paymentLabel, screenWidth / 2 - paymentLabelW / 2, 280, 22, WHITE);
            
            Color cashColor = selectedPayment == 0 ? SKYBLUE : LIGHTGRAY;
            Color cardColor = selectedPayment == 1 ? SKYBLUE : LIGHTGRAY;
            Color mobileColor = selectedPayment == 2 ? SKYBLUE : LIGHTGRAY;
            
            DrawRectangleRec(cashBtn, cashColor);
            DrawRectangleLinesEx(cashBtn, 3, BLACK);
            DrawText("CASH", cashBtn.x + 45, cashBtn.y + 24, 22, BLACK);
            
            DrawRectangleRec(cardBtn, cardColor);
            DrawRectangleLinesEx(cardBtn, 3, BLACK);
            DrawText("CARD", cardBtn.x + 45, cardBtn.y + 24, 22, BLACK);
            
            DrawRectangleRec(mobileBtn, mobileColor);
            DrawRectangleLinesEx(mobileBtn, 3, BLACK);
            DrawText("MOBILE", mobileBtn.x + 35, mobileBtn.y + 24, 22, BLACK);
            
            Color confirmColor = playerMoney >= total ? GREEN : GRAY;
            DrawRectangleRec(confirmBtn, confirmColor);
            DrawRectangleLinesEx(confirmBtn, 3, BLACK);
            DrawText("CONFIRM", confirmBtn.x + 50, confirmBtn.y + 17, 26, WHITE);
            
            DrawRectangleRec(backBtn, ORANGE);
            DrawRectangleLinesEx(backBtn, 2, BLACK);
            DrawText("BACK", backBtn.x + 30, backBtn.y + 12, 16, WHITE);
        }
        
        if (state == SHOPPING) {
            DrawLine(leftWidth, 0, leftWidth, screenHeight, BLACK);
            DrawLine(leftWidth + middleWidth, 0, leftWidth + middleWidth, screenHeight, BLACK);
        }
        
        DrawFPS(10, 10);
        
        EndDrawing();
    }
    
    for (int i = 0; i < 3; i++) {
        UnloadTexture(plantTextures[i]);
        UnloadTexture(potTextures[i]);
    }
    
    CloseWindow();
    return 0;
}