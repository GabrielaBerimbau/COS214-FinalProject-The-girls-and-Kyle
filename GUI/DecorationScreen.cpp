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
    bool needsUpdate = (hasRibbon != originalHasRibbon) || 
                       (hasPot != originalHasPot) || 
                       (hasPot && selectedPotColor != originalPotColor);
    
    if (needsUpdate) {
        int idx = manager->GetCurrentCartIndex();

        if (originalHasRibbon && !hasRibbon) {
            customer->removeRibbonFromCartItem(idx);
            currentPlant = customer->getPlantFromCart(idx);
        }
        if (originalHasPot && !hasPot) {
            customer->removePotFromCartItem(idx);
            currentPlant = customer->getPlantFromCart(idx);
        }
        if (hasPot && originalHasPot && (selectedPotColor != originalPotColor)) {
            customer->removePotFromCartItem(idx);
            currentPlant = customer->getPlantFromCart(idx);
        }

        if (hasRibbon && !originalHasRibbon) {
            customer->decorateCartItemWithRibbon(idx);
            currentPlant = customer->getPlantFromCart(idx);
        }
        if (hasPot && (!originalHasPot || selectedPotColor != originalPotColor)) {
            customer->decorateCartItemWithPot(idx, selectedPotColor);
            currentPlant = customer->getPlantFromCart(idx);
        }
    }
    
    currentPlant = nullptr;
    manager->SwitchScreen(GameScreen::CART_VIEW);
}

void DecorationScreen::HandleBack() {
    std::cout << "[DecorationScreen] Cancelling decoration changes" << std::endl;
    currentPlant = nullptr;
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

    if (hasRibbon) { price += 15.0; }
    if (hasPot)    { price += 80.0; }

    return price;
}

double DecorationScreen::CalculateOriginalPrice() {
    if (currentPlant == nullptr) return 0.0;
    return currentPlant->getPrice();
}

void DecorationScreen::Draw() {
    Color bgColor = manager->IsAlternativeColors()
        ? Color{30, 30, 35, 255}     // Dark mode
        : Color{216, 228, 220, 255}; // Soft sage green
    ClearBackground(bgColor);

    // Header
    const char* header = "DECORATE YOUR PLANT";
    int headerSize = 32;
    int headerWidth = MeasureText(header, headerSize);
    Color headerColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{45, 59, 53, 255};    // Dark green
    DrawText(header, screenWidth / 2 - headerWidth / 2, 30, headerSize, headerColor);

    DrawPlantPreview();
    DrawPotSelection();
    DrawRibbonOption();
    DrawPriceInfo();
    DrawButtons();
}

void DecorationScreen::DrawPlantPreview() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || currentPlant == nullptr) return;
    
    // Preview box
    Rectangle previewBox = Rectangle{
        static_cast<float>(screenWidth / 2 - 200),
        100,
        400,
        250
    };

    Color previewBg = manager->IsAlternativeColors()
        ? Color{50, 55, 55, 255}     // Dark preview box
        : Color{234, 238, 236, 255}; // Light preview box
    Color previewBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint border
        : Color{120, 120, 120, 255}; // Grey border
    DrawRectangleRec(previewBox, previewBg);
    DrawRectangleLinesEx(previewBox, 3, previewBorder);

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
                int rx = (int)(previewBox.x + (previewBox.width - rw) / 2+5);
                // was: int ry = (int)(py + sh * 0.18f);
                int ry = (int)(py + sh * 0.32f);  // lower on stem/pot
                DrawTextureEx(ribbonTexture, Vector2{(float)rx, (float)ry}, 0.0f, rs, WHITE);
            }
        }
    }
    
    // Remove buttons if decorations exist
    Color removeButtonBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : BLACK;                     // Black
    Color removeTextColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : WHITE;                     // White text

    if (hasRibbon) {
        Color removeBase = manager->IsAlternativeColors()
            ? Color{90, 50, 50, 255}     // Dark red
            : Color{174, 83, 70, 255};   // Brown-red
        Color removeHover = manager->IsAlternativeColors()
            ? Color{120, 60, 60, 255}    // Lighter dark red
            : RED;                       // Red
        Color removeColor = removeRibbonHovered ? removeHover : removeBase;
        DrawRectangleRec(removeRibbonButton, removeColor);
        DrawRectangleLinesEx(removeRibbonButton, 2, removeButtonBorder);
        const char* removeText = "Remove Ribbon";
        int textWidth = MeasureText(removeText, 14);
        DrawText(removeText,
                 removeRibbonButton.x + (removeRibbonButton.width - textWidth) / 2,
                 removeRibbonButton.y + (removeRibbonButton.height - 14) / 2,
                 14,
                 removeTextColor);
    }

    if (hasPot) {
        Color removeBase = manager->IsAlternativeColors()
            ? Color{90, 50, 50, 255}     // Dark red
            : Color{174, 83, 70, 255};   // Brown-red
        Color removeHover = manager->IsAlternativeColors()
            ? Color{120, 60, 60, 255}    // Lighter dark red
            : RED;                       // Red
        Color removeColor = removePotHovered ? removeHover : removeBase;
        DrawRectangleRec(removePotButton, removeColor);
        DrawRectangleLinesEx(removePotButton, 2, removeButtonBorder);
        const char* removeText = "Remove Pot";
        int textWidth = MeasureText(removeText, 14);
        DrawText(removeText,
                 removePotButton.x + (removePotButton.width - textWidth) / 2,
                 removePotButton.y + (removePotButton.height - 14) / 2,
                 14,
                 removeTextColor);
    }
}

void DecorationScreen::DrawPotSelection() {
    const char* label = "Select Pot Colour:";
    int labelWidth = MeasureText(label, 20);
    Color labelColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{45, 59, 53, 255};    // Dark green
    DrawText(label, screenWidth / 2 - labelWidth / 2, 355, 20, labelColor);

    // Pot buttons
    for (int i = 0; i < 10; i++) {
        bool isSelected = (hasPot && selectedPotColor == potColors[i]);

        Color btnIdle = manager->IsAlternativeColors()
            ? Color{60, 65, 60, 255}     // Dark grey
            : Color{210, 210, 210, 255}; // Light grey
        Color btnHover = manager->IsAlternativeColors()
            ? Color{75, 80, 75, 255}     // Lighter dark grey
            : Color{230, 230, 230, 255}; // Very light grey
        Color btnSelected = manager->IsAlternativeColors()
            ? Color{80, 90, 60, 255}     // Dark yellow-ish
            : Color{255, 247, 204, 255}; // Light yellow

        Color btnColor = isSelected ? btnSelected : (potHovered[i] ? btnHover : btnIdle);

        Color btnBorderSelected = manager->IsAlternativeColors()
            ? Color{255, 180, 160, 255}  // Light terracotta
            : Color{199, 102, 87, 255};  // Brown-red
        Color btnBorderNormal = manager->IsAlternativeColors()
            ? Color{150, 220, 180, 255}  // Light mint
            : Color{45, 59, 53, 255};    // Dark green

        DrawRectangleRec(potButtons[i], btnColor);
        DrawRectangleLinesEx(potButtons[i], isSelected ? 4 : 2,
                             isSelected ? btnBorderSelected : btnBorderNormal);
        
        // Pot texture preview
        Texture2D potTexture = manager->GetPotTexture(potColors[i]);
        if (potTexture.id != 0) {
            float scale = 70.0f / potTexture.width;
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
    Color ribbonIdle = manager->IsAlternativeColors()
        ? Color{60, 80, 70, 255}     // Dark teal-ish
        : Color{206, 237, 223, 255}; // Soft mint
    Color ribbonHover = manager->IsAlternativeColors()
        ? Color{70, 95, 85, 255}     // Lighter teal-ish
        : Color{164, 211, 194, 255}; // Darker mint
    Color ribbonSelected = manager->IsAlternativeColors()
        ? Color{75, 100, 90, 255}    // Selected teal
        : Color{206, 237, 223, 255}; // Soft mint
    Color ribbonColor = hasRibbon ? ribbonSelected : (ribbonToggleHovered ? ribbonHover : ribbonIdle);
    DrawRectangleRec(ribbonToggleButton, ribbonColor);

    Color ribbonBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : BLACK;                     // Black
    DrawRectangleLinesEx(ribbonToggleButton, 3, ribbonBorder);

    const char* ribbonText = hasRibbon ? "Ribbon Added" : "Add Ribbon";
    int textWidth = MeasureText(ribbonText, 20);
    Color ribbonTextColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : Color{45, 59, 53, 255};    // Dark green
    DrawText(ribbonText,
             ribbonToggleButton.x + (ribbonToggleButton.width - textWidth) / 2,
             ribbonToggleButton.y + (ribbonToggleButton.height - 20) / 2,
             20,
             ribbonTextColor);
}

void DecorationScreen::DrawPriceInfo() {
    int priceY = screenHeight - 200;

    Color originalPriceColor = manager->IsAlternativeColors()
        ? Color{180, 200, 190, 255}  // Light grey-green
        : Color{86, 110, 100, 255};  // Medium dark green

    // Original price
    std::ostringstream originalStream;
    originalStream << "Original Price: R" << std::fixed << std::setprecision(2) << CalculateOriginalPrice();
    int originalWidth = MeasureText(originalStream.str().c_str(), 18);
    DrawText(originalStream.str().c_str(), screenWidth / 2 - originalWidth / 2, priceY, 18, originalPriceColor);

    // New price with decorations
    double newPrice = CalculateCurrentPrice();
    double priceDiff = newPrice - CalculateOriginalPrice();

    Color newPriceColor = manager->IsAlternativeColors()
        ? Color{220, 190, 100, 255}  // Light gold
        : Color{170, 133, 35, 255};  // Dark gold

    std::ostringstream newStream;
    newStream << "New Price: R" << std::fixed << std::setprecision(2) << newPrice;
    int newWidth = MeasureText(newStream.str().c_str(), 24);
    DrawText(newStream.str().c_str(), screenWidth / 2 - newWidth / 2, priceY + 30, 24, newPriceColor);

    // Price difference
    if (priceDiff > 0.01) {
        std::ostringstream diffStream;
        diffStream << "(+" << std::fixed << std::setprecision(2) << priceDiff << ")";
        int diffWidth = MeasureText(diffStream.str().c_str(), 16);
        DrawText(diffStream.str().c_str(), screenWidth / 2 - diffWidth / 2, priceY + 60, 16, newPriceColor);
    }
}

void DecorationScreen::DrawButtons() {
    Color buttonBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : BLACK;                     // Black

    // Confirm button (mint)
    Color confirmIdle = manager->IsAlternativeColors()
        ? Color{60, 80, 70, 255}     // Dark teal-ish
        : Color{206, 237, 223, 255}; // Soft mint
    Color confirmHover = manager->IsAlternativeColors()
        ? Color{70, 95, 85, 255}     // Lighter teal-ish
        : Color{164, 211, 194, 255}; // Darker mint
    Color confirmColor = confirmHovered ? confirmHover : confirmIdle;
    DrawRectangleRec(confirmButton, confirmColor);
    DrawRectangleLinesEx(confirmButton, 3, buttonBorder);
    const char* confirmText = "CONFIRM";
    int confirmWidth = MeasureText(confirmText, 22);
    Color confirmTextColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : Color{45, 59, 53, 255};    // Dark green
    DrawText(confirmText,
             confirmButton.x + (confirmButton.width - confirmWidth) / 2,
             confirmButton.y + (confirmButton.height - 22) / 2,
             22,
             confirmTextColor);

    // Back button (terracotta family)
    Color backIdle = manager->IsAlternativeColors()
        ? Color{90, 50, 50, 255}     // Dark red
        : Color{199, 102, 87, 255};  // Brown-red
    Color backHoverColor = manager->IsAlternativeColors()
        ? Color{110, 65, 60, 255}    // Lighter dark red
        : Color{174, 83, 70, 255};   // Darker brown-red
    Color backColor = backHovered ? backHoverColor : backIdle;
    DrawRectangleRec(backButton, backColor);
    DrawRectangleLinesEx(backButton, 3, buttonBorder);
    const char* backText = "CANCEL";
    int backWidth = MeasureText(backText, 22);
    Color backTextColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : RAYWHITE;                  // White
    DrawText(backText,
             backButton.x + (backButton.width - backWidth) / 2,
             backButton.y + (backButton.height - 22) / 2,
             22,
             backTextColor);
}
