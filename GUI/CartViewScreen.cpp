#include "CartViewScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Backend includes
#include "../include/Customer.h"
#include "../include/Plant.h"
#include "../include/RibbonDecorator.h"
#include "../include/DecorativePotDecorator.h"
#include "../include/Decorator.h"

CartViewScreen::CartViewScreen(ScreenManager* mgr)
    : manager(mgr),
      currentIndex(0),
      leftArrowHovered(false),
      rightArrowHovered(false),
      decorateHovered(false),
      backHovered(false) {
    
    InitializeLayout();
    InitializeButtons();
}

CartViewScreen::~CartViewScreen() {
}

void CartViewScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
}

void CartViewScreen::InitializeButtons() {
    int buttonWidth = 250;
    int buttonHeight = 60;

    // Center decorate button at bottom of screen
    int buttonY = screenHeight - 150;

    decorateButton = Rectangle{
        static_cast<float>(screenWidth / 2 - buttonWidth / 2),
        static_cast<float>(buttonY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    backToSalesFloorButton = Rectangle{
        static_cast<float>(screenWidth / 2 - 120),
        static_cast<float>(screenHeight - 70),
        240,
        50
    };
    
    // Arrow buttons (left and right of plant display)
    int arrowSize = 80;
    int arrowY = screenHeight / 2 - arrowSize / 2;
    
    leftArrowButton = Rectangle{
        100,
        static_cast<float>(arrowY),
        static_cast<float>(arrowSize),
        static_cast<float>(arrowSize)
    };
    
    rightArrowButton = Rectangle{
        static_cast<float>(screenWidth - 100 - arrowSize),
        static_cast<float>(arrowY),
        static_cast<float>(arrowSize),
        static_cast<float>(arrowSize)
    };
}

void CartViewScreen::Update() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || customer->getCartSize() == 0) {
        // If cart is empty, go back to sales floor
        manager->SwitchScreen(GameScreen::SALES_FLOOR);
        return;
    }
    
    // Ensure currentIndex is valid
    if (currentIndex >= customer->getCartSize()) {
        currentIndex = customer->getCartSize() - 1;
    }
    if (currentIndex < 0) {
        currentIndex = 0;
    }
    
    UpdateButtons();
}

void CartViewScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();
    
    Customer* customer = manager->GetCustomer();
    bool hasMultipleItems = (customer != nullptr && customer->getCartSize() > 1);
    
    leftArrowHovered = hasMultipleItems && CheckCollisionPointRec(mousePos, leftArrowButton);
    rightArrowHovered = hasMultipleItems && CheckCollisionPointRec(mousePos, rightArrowButton);
    decorateHovered = CheckCollisionPointRec(mousePos, decorateButton);
    backHovered = CheckCollisionPointRec(mousePos, backToSalesFloorButton);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (leftArrowHovered) {
            HandleLeftArrow();
        } else if (rightArrowHovered) {
            HandleRightArrow();
        } else if (decorateHovered) {
            HandleDecorate();
        } else if (backHovered) {
            HandleBack();
        }
    }
}

void CartViewScreen::HandleLeftArrow() {
    Customer* customer = manager->GetCustomer();
    if (customer != nullptr && customer->getCartSize() > 0) {
        currentIndex--;
        if (currentIndex < 0) {
            currentIndex = customer->getCartSize() - 1; // Wrap around
        }
        std::cout << "[CartViewScreen] Navigated to cart item " << currentIndex << std::endl;
    }
}

void CartViewScreen::HandleRightArrow() {
    Customer* customer = manager->GetCustomer();
    if (customer != nullptr && customer->getCartSize() > 0) {
        currentIndex++;
        if (currentIndex >= customer->getCartSize()) {
            currentIndex = 0; // Wrap around
        }
        std::cout << "[CartViewScreen] Navigated to cart item " << currentIndex << std::endl;
    }
}

void CartViewScreen::HandleDecorate() {
    Customer* customer = manager->GetCustomer();
    if (customer != nullptr && currentIndex >= 0 && currentIndex < customer->getCartSize()) {
        // Store which plant we're decorating
        manager->SetCurrentCartIndex(currentIndex);
        
        std::cout << "[CartViewScreen] Opening decoration screen for cart item " << currentIndex << std::endl;
        manager->SwitchScreen(GameScreen::DECORATION);
    }
}

void CartViewScreen::HandleBack() {
    std::cout << "[CartViewScreen] Returning to sales floor" << std::endl;
    manager->SwitchScreen(GameScreen::SALES_FLOOR);
}

void CartViewScreen::Draw() {
    // Light mode: Soft sage green | Dark mode: Dark background
    Color bgColor = manager->IsAlternativeColors()
        ? Color{30, 30, 35, 255}     // Dark mode
        : Color{216, 228, 220, 255}; // Light sage green
    ClearBackground(bgColor);

    // Draw header
    const char* header = "YOUR CART";
    int headerSize = 36;
    int headerWidth = MeasureText(header, headerSize);
    Color headerColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint for dark mode
        : Color{45, 59, 53, 255};    // Deeper forest
    DrawText(header, screenWidth / 2 - headerWidth / 2, 30, headerSize, headerColor);

    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || customer->getCartSize() == 0) {
        const char* emptyText = "Cart is empty!";
        int emptyWidth = MeasureText(emptyText, 24);
        Color emptyColor = manager->IsAlternativeColors()
            ? Color{160, 160, 160, 255}  // Light grey
            : Color{86, 110, 100, 255};  // Mid-sage
        DrawText(emptyText, screenWidth / 2 - emptyWidth / 2, screenHeight / 2, 24, emptyColor);
        return;
    }

    // Draw cart counter
    std::ostringstream counterStream;
    counterStream << "Item " << (currentIndex + 1) << " of " << customer->getCartSize();
    std::string counterText = counterStream.str();
    int counterWidth = MeasureText(counterText.c_str(), 20);
    Color counterColor = manager->IsAlternativeColors()
        ? Color{255, 150, 130, 255}  // Light coral for dark mode
        : Color{199, 102, 87, 255};  // Terracotta
    DrawText(counterText.c_str(), screenWidth / 2 - counterWidth / 2, 90, 20, counterColor);
    
    DrawPlantDisplay();
    DrawPlantInfo();
    DrawNavigationArrows();
    DrawButtons();
}

void CartViewScreen::DrawPlantDisplay() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;
    
    Plant* plant = customer->getPlantFromCart(currentIndex);
    if (plant == nullptr) return;
    
    // Draw display box
    Rectangle displayBox = Rectangle{
        static_cast<float>(screenWidth / 2 - 250),
        150,
        500,
        400
    };
    
    // Card colors
    Color cardFill = manager->IsAlternativeColors()
        ? Color{45, 45, 50, 255}     // Dark card
        : Color{234, 238, 236, 255}; // Light card
    Color cardBorder = manager->IsAlternativeColors()
        ? Color{80, 80, 85, 255}     // Lighter grey for dark mode
        : Color{120, 120, 120, 255}; // Dark grey
    DrawRectangleRec(displayBox, cardFill);
    DrawRectangleLinesEx(displayBox, 3, cardBorder);

    bool hasRibbon = false, hasPot = false;
    std::string potColor;

    Plant* walker = plant;
    while (auto dec = dynamic_cast<Decorator*>(walker)) {
        if (auto pot = dynamic_cast<DecorativePotDecorator*>(walker)) { hasPot = true; potColor = pot->getPotColor(); }
        if (dynamic_cast<RibbonDecorator*>(walker)) { hasRibbon = true; }
        walker = dec->getWrappedPlant();
    }
    Plant* basePlant = walker ? walker : plant;

    Texture2D plantTexture = manager->GetPlantTexture(basePlant->getName());
    
    if (plantTexture.id != 0) {
        // Scale plant to fit in display
        float scale = 300.0f / plantTexture.height;
        int scaledWidth = static_cast<int>(plantTexture.width * scale);
        int scaledHeight = static_cast<int>(plantTexture.height * scale);
        
        int plantX = displayBox.x + (displayBox.width - scaledWidth) / 2;
        int plantY = displayBox.y + (displayBox.height - scaledHeight) / 2 - 20;
        
        DrawTextureEx(plantTexture, Vector2{static_cast<float>(plantX), static_cast<float>(plantY)}, 
                     0.0f, scale, WHITE);

        // Draw pot overlay if present
        if (hasPot && !potColor.empty()) {
            Texture2D potTexture = manager->GetPotTexture(potColor);
            if (potTexture.id != 0) {
                float potScale = 320.0f / potTexture.width;
                int potWidth  = static_cast<int>(potTexture.width  * potScale);
                int potHeight = static_cast<int>(potTexture.height * potScale);
                int potX = displayBox.x + (displayBox.width - potWidth) / 2;
                int potY = plantY + scaledHeight - potHeight + 40;
                DrawTextureEx(potTexture, Vector2{static_cast<float>(potX), static_cast<float>(potY)}, 0.0f, potScale, WHITE);
            }
        }

        // Draw ribbon overlay if present
        if (hasRibbon) {
            Texture2D ribbonTexture = manager->GetRibbonTexture();
            if (ribbonTexture.id != 0) {
                float rs = 150.0f / ribbonTexture.width;
                int rw = static_cast<int>(ribbonTexture.width * rs);
                int rh = static_cast<int>(ribbonTexture.height * rs);
                int rx = static_cast<int>(displayBox.x + (displayBox.width - rw) / 2 + 5);
                int ry = static_cast<int>(plantY + scaledHeight * 0.4f);
                DrawTextureEx(ribbonTexture, Vector2{static_cast<float>(rx), static_cast<float>(ry)}, 0.0f, rs, WHITE);
            }
        }
    } else {
        // Fallback: draw plant name
        const char* name = plant->getName().c_str();
        int nameWidth = MeasureText(name, 24);
        DrawText(name, displayBox.x + (displayBox.width - nameWidth) / 2, 
                displayBox.y + displayBox.height / 2, 24, WHITE);
    }
}

void CartViewScreen::DrawPlantInfo() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;
    
    Plant* plant = customer->getPlantFromCart(currentIndex);
    if (plant == nullptr) return;
    
    int infoX = screenWidth / 2 - 200;
    int infoY = 580;
    
    // Plant name
    std::string nameText = plant->getName();
    Color nameColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{45, 59, 53, 255};    // Deeper forest
    DrawText(nameText.c_str(), infoX, infoY, 24, nameColor);
    infoY += 35;

    // Plant ID
    std::string idText = "ID: " + plant->getID();
    Color idColor = manager->IsAlternativeColors()
        ? Color{140, 140, 140, 255}  // Light grey
        : Color{86, 110, 100, 255};  // Mid-sage
    DrawText(idText.c_str(), infoX, infoY, 16, idColor);
    infoY += 25;

    // Discover decorations by walking the chain
    bool hasRibbon = false, hasPot = false;
    std::string potColor;

    Plant* w = plant;
    while (auto d = dynamic_cast<Decorator*>(w)) {
        if (auto p = dynamic_cast<DecorativePotDecorator*>(w)) { hasPot = true; potColor = p->getPotColor(); }
        if (dynamic_cast<RibbonDecorator*>(w)) { hasRibbon = true; }
        w = d->getWrappedPlant();
    }

    Color decorColor = manager->IsAlternativeColors()
        ? Color{255, 150, 130, 255}  // Light coral
        : Color{199, 102, 87, 255};  // Terracotta
    Color potDecorColor = manager->IsAlternativeColors()
        ? Color{130, 170, 220, 255}  // Light blue
        : Color{88, 118, 159, 255};  // Slate blue

    if (hasRibbon || hasPot) {
        DrawText("Decorations:", infoX, infoY, 18, decorColor);
        infoY += 25;

        if (hasRibbon) {
            DrawText("- Ribbon", infoX + 20, infoY, 16, decorColor);
            infoY += 22;
        }
        if (hasPot) {
            std::string potText = "- " + potColor + " Pot";
            DrawText(potText.c_str(), infoX + 20, infoY, 16, potDecorColor);
            infoY += 22;
        }
        infoY += 10;
    }

    // Price (large and highlighted)
    std::ostringstream priceStream;
    priceStream << "Price: R" << std::fixed << std::setprecision(2) << plant->getPrice();
    Color priceColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{45, 59, 53, 255};    // Deeper forest
    DrawText(priceStream.str().c_str(), infoX, infoY, 28, priceColor);
}

void CartViewScreen::DrawNavigationArrows() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || customer->getCartSize() <= 1) return;

    // Colors
    Color fillIdle = manager->IsAlternativeColors()
        ? Color{50, 50, 55, 255}     // Dark button
        : Color{230, 240, 235, 255}; // Soft pastel
    Color fillHover = manager->IsAlternativeColors()
        ? Color{65, 65, 70, 255}     // Lighter on hover
        : Color{210, 237, 235, 255}; // Hover pastel
    Color border = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint border
        : Color{45, 59, 53, 255};    // Dark forest green

    // --- LEFT ARROW ---
    Color leftColor = leftArrowHovered ? fillHover : fillIdle;
    DrawRectangleRec(leftArrowButton, leftColor);
    DrawRectangleLinesEx(leftArrowButton, 3, border);

    // Hollow triangle (outline only)
    float pad = 20.0f;
    Vector2 L1 = { leftArrowButton.x + leftArrowButton.width  - pad, leftArrowButton.y + pad };
    Vector2 L2 = { leftArrowButton.x + leftArrowButton.width  - pad, leftArrowButton.y + leftArrowButton.height - pad };
    Vector2 L3 = { leftArrowButton.x + pad,                     leftArrowButton.y + leftArrowButton.height / 2.0f };
    DrawTriangleLines(L1, L2, L3, border);

    // --- RIGHT ARROW ---
    Color rightColor = rightArrowHovered ? fillHover : fillIdle;
    DrawRectangleRec(rightArrowButton, rightColor);
    DrawRectangleLinesEx(rightArrowButton, 3, border);

    Vector2 R1 = { rightArrowButton.x + pad, rightArrowButton.y + pad };
    Vector2 R2 = { rightArrowButton.x + pad, rightArrowButton.y + rightArrowButton.height - pad };
    Vector2 R3 = { rightArrowButton.x + rightArrowButton.width - pad, rightArrowButton.y + rightArrowButton.height / 2.0f };
    DrawTriangleLines(R1, R2, R3, border);
}



void CartViewScreen::DrawButtons() {
    Color border = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{45, 59, 53, 255};    // Deeper forest
    Color textColor = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : Color{45, 59, 53, 255};    // Dark text

    // Decorate button
    Color decorateIdle = manager->IsAlternativeColors()
        ? Color{55, 45, 65, 255}     // Dark purple
        : Color{230, 224, 237, 255}; // Soft lavender
    Color decorateHover = manager->IsAlternativeColors()
        ? Color{70, 55, 85, 255}     // Lighter purple
        : Color{215, 195, 220, 255}; // Lavender hover
    Color decorateColor = decorateHovered ? decorateHover : decorateIdle;
    DrawRectangleRec(decorateButton, decorateColor);
    DrawRectangleLinesEx(decorateButton, 3, border);
    const char* decorateText = "DECORATE PLANT";
    int decorateTextWidth = MeasureText(decorateText, 22);
    DrawText(decorateText,
             decorateButton.x + (decorateButton.width - decorateTextWidth) / 2,
             decorateButton.y + (decorateButton.height - 22) / 2,
             22,
             textColor);

    // Back button
    Color backIdle = manager->IsAlternativeColors()
        ? Color{50, 50, 55, 255}     // Dark grey
        : Color{245, 240, 242, 255}; // Light pink-grey
    Color backHoverColor = manager->IsAlternativeColors()
        ? Color{65, 65, 70, 255}     // Lighter grey
        : Color{235, 225, 230, 255}; // Darker pink-grey
    Color backColor = backHovered ? backHoverColor : backIdle;
    DrawRectangleRec(backToSalesFloorButton, backColor);
    DrawRectangleLinesEx(backToSalesFloorButton, 2, border);
    const char* backText = "Back to Sales Floor";
    int backTextWidth = MeasureText(backText, 18);
    DrawText(backText,
             backToSalesFloorButton.x + (backToSalesFloorButton.width - backTextWidth) / 2,
             backToSalesFloorButton.y + (backToSalesFloorButton.height - 18) / 2,
             18,
             textColor);
}
