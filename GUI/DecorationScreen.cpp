#include "DecorationScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Backend includes
#include "../include/Customer.h"
#include "../include/Plant.h"
#include "../include/Decorator.h"
#include "../include/RibbonDecorator.h"
#include "../include/DecorativePotDecorator.h"

DecorationScreen::DecorationScreen(ScreenManager* mgr)
    : manager(mgr),
      cartIndex(0),
      currentPlant(nullptr),
      selectedPotColor(""),
      hasRibbon(false),
      hasPot(false),
      originalPotColor(""),
      originalHasRibbon(false),
      originalHasPot(false),
      ribbonToggleHovered(false),
      removeRibbonHovered(false),
      removePotHovered(false),
      confirmHovered(false),
      backHovered(false) {
    
    for (int i = 0; i < 10; i++) {
        potHovered[i] = false;
    }
    
    InitializeLayout();
    InitializeButtons();
}

DecorationScreen::~DecorationScreen() {
}

void DecorationScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
}

void DecorationScreen::InitializeButtons() {
    // Pot buttons - 2 rows of 5
    int potBtnSize = 80;  // Reduced from 100 to 80
    int potBtnSpacing = 15;  // Reduced from 20 to 15
    int totalPotWidth = (potBtnSize * 5) + (potBtnSpacing * 4);
    int potStartX = (screenWidth - totalPotWidth) / 2;
    int potRow1Y = 380;  // Moved up from 420
    int potRow2Y = potRow1Y + potBtnSize + potBtnSpacing;
    
    for (int i = 0; i < 5; i++) {
        potButtons[i] = Rectangle{
            static_cast<float>(potStartX + i * (potBtnSize + potBtnSpacing)),
            static_cast<float>(potRow1Y),
            static_cast<float>(potBtnSize),
            static_cast<float>(potBtnSize)
        };
    }
    
    for (int i = 5; i < 10; i++) {
        potButtons[i] = Rectangle{
            static_cast<float>(potStartX + (i - 5) * (potBtnSize + potBtnSpacing)),
            static_cast<float>(potRow2Y),
            static_cast<float>(potBtnSize),
            static_cast<float>(potBtnSize)
        };
    }
    
    // Ribbon toggle button
    ribbonToggleButton = Rectangle{
        static_cast<float>(screenWidth / 2 - 120),
        static_cast<float>(potRow2Y + potBtnSize + 25),  // Reduced spacing from 40 to 25
        240,
        50
    };

    // Remove decoration buttons (positioned to the right of preview, well-spaced)
    removeRibbonButton = Rectangle{
        static_cast<float>(screenWidth / 2 + 220),  // Further right to avoid overlap
        140,  // Higher up
        150,  // Wider
        45    // Taller
    };

    removePotButton = Rectangle{
        static_cast<float>(screenWidth / 2 + 220),  // Same x as ribbon button
        195,  // Below ribbon button with spacing
        150,  // Same width as ribbon button
        45    // Same height as ribbon button
    };
    
    // Confirm and Back buttons
    confirmButton = Rectangle{
        static_cast<float>(screenWidth / 2 - 230),
        static_cast<float>(screenHeight - 80),
        200,
        50
    };
    
    backButton = Rectangle{
        static_cast<float>(screenWidth / 2 + 30),
        static_cast<float>(screenHeight - 80),
        200,
        50
    };
}

void DecorationScreen::LoadCurrentDecorations() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;
    
    cartIndex = manager->GetCurrentCartIndex();
    currentPlant = customer->getPlantFromCart(cartIndex);
    
    if (currentPlant == nullptr) return;
    
    // Check if plant has decorations
    selectedPotColor = "";
    hasRibbon = false;
    hasPot = false;


    Plant* walker = currentPlant;
    while (auto dec = dynamic_cast<Decorator*>(walker)) {
        if (auto pot = dynamic_cast<DecorativePotDecorator*>(walker)) { hasPot = true; selectedPotColor = pot->getPotColor(); }
        if (dynamic_cast<RibbonDecorator*>(walker)) { hasRibbon = true; }
        walker = dec->getWrappedPlant();
    }

    
    // // Check for pot decorator
    // DecorativePotDecorator* potDec = dynamic_cast<DecorativePotDecorator*>(currentPlant);
    // if (potDec != nullptr) {
    //     selectedPotColor = potDec->getPotColor();
    //     hasPot = true;
    // }
    
    // // Check for ribbon decorator
    // RibbonDecorator* ribbonDec = dynamic_cast<RibbonDecorator*>(currentPlant);
    // if (ribbonDec != nullptr) {
    //     hasRibbon = true;
    // }
    
    // Store original state
    originalPotColor = selectedPotColor;
    originalHasRibbon = hasRibbon;
    originalHasPot = hasPot;
    
    std::cout << "[DecorationScreen] Loaded plant decorations - Pot: " 
              << (hasPot ? selectedPotColor : "none") 
              << ", Ribbon: " << (hasRibbon ? "yes" : "no") << std::endl;
}

void DecorationScreen::Update() {
    // Load current plant state on first frame
    if (currentPlant == nullptr) {
        LoadCurrentDecorations();
    }
    
    // If still no plant, go back to cart
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || currentPlant == nullptr) {
        manager->SwitchScreen(GameScreen::CART_VIEW);
        return;
    }
    
    UpdateButtons();
}

void DecorationScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();
    
    // Pot buttons
    for (int i = 0; i < 10; i++) {
        potHovered[i] = CheckCollisionPointRec(mousePos, potButtons[i]);
    }
    
    ribbonToggleHovered = CheckCollisionPointRec(mousePos, ribbonToggleButton);
    removeRibbonHovered = hasRibbon && CheckCollisionPointRec(mousePos, removeRibbonButton);
    removePotHovered = hasPot && CheckCollisionPointRec(mousePos, removePotButton);
    confirmHovered = CheckCollisionPointRec(mousePos, confirmButton);
    backHovered = CheckCollisionPointRec(mousePos, backButton);
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < 10; i++) {
            if (potHovered[i]) {
                HandlePotSelection(i);
                break;
            }
        }
        
        if (ribbonToggleHovered) {
            HandleRibbonToggle();
        } else if (removeRibbonHovered) {
            HandleRemoveRibbon();
        } else if (removePotHovered) {
            HandleRemovePot();
        } else if (confirmHovered) {
            HandleConfirm();
        } else if (backHovered) {
            HandleBack();
        }
    }
}

void DecorationScreen::HandlePotSelection(int potIndex) {
    selectedPotColor = potColors[potIndex];
    hasPot = true;
    std::cout << "[DecorationScreen] Selected pot color: " << selectedPotColor << std::endl;
}

void DecorationScreen::HandleRibbonToggle() {
    hasRibbon = !hasRibbon;
    std::cout << "[DecorationScreen] Ribbon toggled: " << (hasRibbon ? "ON" : "OFF") << std::endl;
}

void DecorationScreen::HandleRemoveRibbon() {
    hasRibbon = false;
    std::cout << "[DecorationScreen] Ribbon removed" << std::endl;
}

void DecorationScreen::HandleRemovePot() {
    hasPot = false;
    selectedPotColor = "";
    std::cout << "[DecorationScreen] Pot removed" << std::endl;
}

void DecorationScreen::HandleConfirm() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || currentPlant == nullptr) return;
    
    std::cout << "[DecorationScreen] Confirming decorations..." << std::endl;
    
    // Apply decorations using backend
    // First remove existing decorations if changed
    bool needsUpdate = (hasRibbon != originalHasRibbon) || 
                       (hasPot != originalHasPot) || 
                       (hasPot && selectedPotColor != originalPotColor);
    
    if (needsUpdate) {
        // The cart index might have changed if decorators were added/removed
        // So we need to refresh the plant reference
        int idx = manager->GetCurrentCartIndex();
        
        // Remove old ribbon if needed
        // if (originalHasRibbon && !hasRibbon) {
        //     std::cout << "[DecorationScreen] Removing old ribbon decoration" << std::endl;
        // }
        
        // // Remove old pot if needed
        // if (originalHasPot && !hasPot) {
        //     std::cout << "[DecorationScreen] Removing old pot decoration" << std::endl;
        // }
        
        // // Add new ribbon if needed
        // if (hasRibbon && !originalHasRibbon) {
        //     customer->decorateCartItemWithRibbon(currentCartIndex);
        //     std::cout << "[DecorationScreen] Added ribbon decoration" << std::endl;
        // }
        
        // // Add new pot if needed (or replace)
        // if (hasPot && (!originalHasPot || selectedPotColor != originalPotColor)) {
        //     customer->decorateCartItemWithPot(currentCartIndex, selectedPotColor);
        //     std::cout << "[DecorationScreen] Added " << selectedPotColor << " pot decoration" << std::endl;
        // }



        if (originalHasRibbon && !hasRibbon) {
            customer->removeRibbonFromCartItem(idx);
            // refresh local view
            currentPlant = customer->getPlantFromCart(idx);
        }
        if (originalHasPot && !hasPot) {
            customer->removePotFromCartItem(idx);
            currentPlant = customer->getPlantFromCart(idx);
        }
        if (hasPot && originalHasPot && (selectedPotColor != originalPotColor)) {
            // replace pot: remove, then add with new color
            customer->removePotFromCartItem(idx);
            currentPlant = customer->getPlantFromCart(idx);
        }

        // Now add new/changed selections
        if (hasRibbon && !originalHasRibbon) {
            customer->decorateCartItemWithRibbon(idx);
            currentPlant = customer->getPlantFromCart(idx);
        }
        if (hasPot && (!originalHasPot || selectedPotColor != originalPotColor)) {
            customer->decorateCartItemWithPot(idx, selectedPotColor);
            currentPlant = customer->getPlantFromCart(idx);
        }
    }
    
    // Reset state
    currentPlant = nullptr;
    
    // Return to cart view
    manager->SwitchScreen(GameScreen::CART_VIEW);
}

void DecorationScreen::HandleBack() {
    std::cout << "[DecorationScreen] Cancelling decoration changes" << std::endl;
    
    // Reset state
    currentPlant = nullptr;
    
    // Return to cart view without applying changes
    manager->SwitchScreen(GameScreen::CART_VIEW);
}

double DecorationScreen::CalculateCurrentPrice() {
    if (currentPlant == nullptr) return 0.0;

    // Get base plant price (unwrap all decorators)
    Plant* basePlant = currentPlant;
    while (auto dec = dynamic_cast<Decorator*>(basePlant)) {
        basePlant = dec->getWrappedPlant();
    }

    double price = basePlant ? basePlant->getPrice() : 0.0;

    // Add decoration costs based on PREVIEW selections
    if (hasRibbon) {
        price += 15.0; // RIBBON_PRICE from RibbonDecorator.h
    }
    if (hasPot) {
        price += 80.0; // POT_PRICE from DecorativePotDecorator.h (was incorrectly 20.0)
    }

    return price;
}

double DecorationScreen::CalculateOriginalPrice() {
    if (currentPlant == nullptr) return 0.0;
    
    return currentPlant->getPrice();
}

void DecorationScreen::Draw() {
    ClearBackground(Color{30, 50, 40, 255});
    
    // Draw header
    const char* header = "DECORATE YOUR PLANT";
    int headerSize = 32;
    int headerWidth = MeasureText(header, headerSize);
    DrawText(header, screenWidth / 2 - headerWidth / 2, 30, headerSize, WHITE);
    
    DrawPlantPreview();
    DrawPotSelection();
    DrawRibbonOption();
    DrawPriceInfo();
    DrawButtons();
}

void DecorationScreen::DrawPlantPreview() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || currentPlant == nullptr) return;
    
    // Draw preview box
    Rectangle previewBox = Rectangle{
        static_cast<float>(screenWidth / 2 - 200),
        100,
        400,
        250
    };
    
    DrawRectangleRec(previewBox, Color{50, 70, 60, 255});
    DrawRectangleLinesEx(previewBox, 3, GOLD);


    Plant* walker = currentPlant;
    while (auto dec = dynamic_cast<Decorator*>(walker)) { walker = dec->getWrappedPlant(); }
    Plant* basePlant = walker ? walker : currentPlant;

    Texture2D plantTexture = manager->GetPlantTexture(basePlant->getName());
    if (plantTexture.id != 0) {
        float scale = 200.0f / plantTexture.height;
        int sw = (int)(plantTexture.width * scale);
        int sh = (int)(plantTexture.height * scale);
        int px = (int)(previewBox.x + (previewBox.width - sw) / 2);
        int py = (int)(previewBox.y + (previewBox.height - sh) / 2 - 10);

        DrawTextureEx(plantTexture, Vector2{(float)px, (float)py}, 0.0f, scale, WHITE);

        if (hasPot && !selectedPotColor.empty()) {
            Texture2D potTexture = manager->GetPotTexture(selectedPotColor);
            if (potTexture.id != 0) {
                float potScale = 220.0f / potTexture.width;
                int pw = (int)(potTexture.width * potScale);
                int ph = (int)(potTexture.height * potScale);
                int potX = (int)(previewBox.x + (previewBox.width - pw) / 2);
                int potY = py + sh - ph + 30;
                DrawTextureEx(potTexture, Vector2{(float)potX, (float)potY}, 0.0f, potScale, WHITE);
            }
        }

        if (hasRibbon) {
            Texture2D ribbonTexture = manager->GetRibbonTexture();
            if (ribbonTexture.id != 0) {
                float rs = 150.0f / ribbonTexture.width;
                int rw = (int)(ribbonTexture.width * rs);
                int rh = (int)(ribbonTexture.height * rs);
                int rx = (int)(previewBox.x + (previewBox.width - rw) / 2);
                int ry = py - 15;
                DrawTextureEx(ribbonTexture, Vector2{(float)rx, (float)ry}, 0.0f, rs, WHITE);
            }
        }
    }


    
    // Get base plant name for texture
    // std::string plantName = currentPlant->getName();
    // Texture2D plantTexture = manager->GetPlantTexture(plantName);
    
    // if (plantTexture.id != 0) {
    //     // Draw plant
    //     float scale = 200.0f / plantTexture.height;
    //     int scaledWidth = static_cast<int>(plantTexture.width * scale);
    //     int scaledHeight = static_cast<int>(plantTexture.height * scale);
        
    //     int plantX = previewBox.x + (previewBox.width - scaledWidth) / 2;
    //     int plantY = previewBox.y + (previewBox.height - scaledHeight) / 2 - 10;
        
    //     DrawTextureEx(plantTexture, Vector2{static_cast<float>(plantX), static_cast<float>(plantY)}, 
    //                  0.0f, scale, WHITE);
        
    //     // Draw pot decoration if selected
    //     if (hasPot && !selectedPotColor.empty()) {
    //         Texture2D potTexture = manager->GetPotTexture(selectedPotColor);
            
    //         if (potTexture.id != 0) {
    //             float potScale = 220.0f / potTexture.width;
    //             int potWidth = static_cast<int>(potTexture.width * potScale);
    //             int potHeight = static_cast<int>(potTexture.height * potScale);
                
    //             int potX = previewBox.x + (previewBox.width - potWidth) / 2;
    //             int potY = plantY + scaledHeight - potHeight + 30;
                
    //             DrawTextureEx(potTexture, Vector2{static_cast<float>(potX), static_cast<float>(potY)},
    //                         0.0f, potScale, WHITE);
    //         }
    //     }
        
    //     // Draw ribbon decoration if selected
    //     if (hasRibbon) {
    //         Texture2D ribbonTexture = manager->GetRibbonTexture();
            
    //         if (ribbonTexture.id != 0) {
    //             float ribbonScale = 150.0f / ribbonTexture.width;
    //             int ribbonWidth = static_cast<int>(ribbonTexture.width * ribbonScale);
    //             int ribbonHeight = static_cast<int>(ribbonTexture.height * ribbonScale);
                
    //             int ribbonX = previewBox.x + (previewBox.width - ribbonWidth) / 2;
    //             int ribbonY = plantY - 15;
                
    //             DrawTextureEx(ribbonTexture, Vector2{static_cast<float>(ribbonX), static_cast<float>(ribbonY)},
    //                         0.0f, ribbonScale, WHITE);
    //         }
    //     }
    // }
    
    // Draw remove buttons if decorations exist
    if (hasRibbon) {
        Color removeColor = removeRibbonHovered ? RED : Color{150, 50, 50, 255};
        DrawRectangleRec(removeRibbonButton, removeColor);
        DrawRectangleLinesEx(removeRibbonButton, 2, BLACK);
        const char* removeText = "Remove Ribbon";
        int textWidth = MeasureText(removeText, 14);
        DrawText(removeText,
                 removeRibbonButton.x + (removeRibbonButton.width - textWidth) / 2,
                 removeRibbonButton.y + (removeRibbonButton.height - 14) / 2,
                 14,
                 WHITE);
    }
    
    if (hasPot) {
        Color removeColor = removePotHovered ? RED : Color{150, 50, 50, 255};
        DrawRectangleRec(removePotButton, removeColor);
        DrawRectangleLinesEx(removePotButton, 2, BLACK);
        const char* removeText = "Remove Pot";
        int textWidth = MeasureText(removeText, 14);
        DrawText(removeText,
                 removePotButton.x + (removePotButton.width - textWidth) / 2,
                 removePotButton.y + (removePotButton.height - 14) / 2,
                 14,
                 WHITE);
    }
}

void DecorationScreen::DrawPotSelection() {
    const char* label = "Select Pot Color:";
    int labelWidth = MeasureText(label, 20);
    DrawText(label, screenWidth / 2 - labelWidth / 2, 355, 20, WHITE);  // Moved up from 380 to 355
    
    // Draw pot buttons
    for (int i = 0; i < 10; i++) {
        bool isSelected = (hasPot && selectedPotColor == potColors[i]);
        Color btnColor = isSelected ? SKYBLUE : (potHovered[i] ? LIGHTGRAY : Color{100, 100, 100, 255});
        
        DrawRectangleRec(potButtons[i], btnColor);
        DrawRectangleLinesEx(potButtons[i], isSelected ? 4 : 2, isSelected ? GOLD : BLACK);
        
        // Draw pot texture preview
        Texture2D potTexture = manager->GetPotTexture(potColors[i]);
        if (potTexture.id != 0) {
            float scale = 70.0f / potTexture.width;  // Adjusted from 80 to 70 to fit in smaller button
            int texWidth = static_cast<int>(potTexture.width * scale);
            int texHeight = static_cast<int>(potTexture.height * scale);
            
            int texX = potButtons[i].x + (potButtons[i].width - texWidth) / 2;
            int texY = potButtons[i].y + (potButtons[i].height - texHeight) / 2;
            
            DrawTextureEx(potTexture, Vector2{static_cast<float>(texX), static_cast<float>(texY)},
                         0.0f, scale, WHITE);
        }
    }
}

void DecorationScreen::DrawRibbonOption() {
    Color ribbonColor = hasRibbon ? GREEN : (ribbonToggleHovered ? DARKGRAY : GRAY);
    DrawRectangleRec(ribbonToggleButton, ribbonColor);
    DrawRectangleLinesEx(ribbonToggleButton, 3, BLACK);
    
    const char* ribbonText = hasRibbon ? "✓ Ribbon Added" : "Add Ribbon";
    int textWidth = MeasureText(ribbonText, 20);
    DrawText(ribbonText,
             ribbonToggleButton.x + (ribbonToggleButton.width - textWidth) / 2,
             ribbonToggleButton.y + (ribbonToggleButton.height - 20) / 2,
             20,
             WHITE);
}

void DecorationScreen::DrawPriceInfo() {
    int priceY = screenHeight - 150;
    
    // Original price
    std::ostringstream originalStream;
    originalStream << "Original Price: R" << std::fixed << std::setprecision(2) << CalculateOriginalPrice();
    int originalWidth = MeasureText(originalStream.str().c_str(), 18);
    DrawText(originalStream.str().c_str(), screenWidth / 2 - originalWidth / 2, priceY, 18, LIGHTGRAY);
    
    // New price with decorations
    double newPrice = CalculateCurrentPrice();
    double priceDiff = newPrice - CalculateOriginalPrice();
    
    std::ostringstream newStream;
    newStream << "New Price: R" << std::fixed << std::setprecision(2) << newPrice;
    int newWidth = MeasureText(newStream.str().c_str(), 24);
    DrawText(newStream.str().c_str(), screenWidth / 2 - newWidth / 2, priceY + 30, 24, GOLD);
    
    // Price difference
    if (priceDiff > 0.01) {
        std::ostringstream diffStream;
        diffStream << "(+" << std::fixed << std::setprecision(2) << priceDiff << ")";
        int diffWidth = MeasureText(diffStream.str().c_str(), 16);
        DrawText(diffStream.str().c_str(), screenWidth / 2 - diffWidth / 2, priceY + 60, 16, YELLOW);
    }
}

void DecorationScreen::DrawButtons() {
    // Confirm button
    Color confirmColor = confirmHovered ? DARKGREEN : GREEN;
    DrawRectangleRec(confirmButton, confirmColor);
    DrawRectangleLinesEx(confirmButton, 3, BLACK);
    const char* confirmText = "CONFIRM";
    int confirmWidth = MeasureText(confirmText, 22);
    DrawText(confirmText,
             confirmButton.x + (confirmButton.width - confirmWidth) / 2,
             confirmButton.y + (confirmButton.height - 22) / 2,
             22,
             WHITE);
    
    // Back button
    Color backColor = backHovered ? Color{150, 50, 50, 255} : Color{100, 50, 50, 255};
    DrawRectangleRec(backButton, backColor);
    DrawRectangleLinesEx(backButton, 3, BLACK);
    const char* backText = "CANCEL";
    int backWidth = MeasureText(backText, 22);
    DrawText(backText,
             backButton.x + (backButton.width - backWidth) / 2,
             backButton.y + (backButton.height - 22) / 2,
             22,
             WHITE);
}