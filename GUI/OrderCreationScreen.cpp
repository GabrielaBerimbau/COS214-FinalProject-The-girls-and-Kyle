#include "OrderCreationScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

// Backend includes
#include "../include/Customer.h"
#include "../include/Plant.h"
#include "../include/ConcreteOrder.h"
#include "../include/Leaf.h"
#include "../include/FinalOrder.h"

OrderCreationScreen::OrderCreationScreen(ScreenManager* mgr)
    : manager(mgr),
      rootOrderNode(nullptr),
      selectedOrderNode(nullptr),
      isCreatingSuborder(false),
      maxSuborderNameLength(30),
      cartScrollOffset(0.0f),
      orderScrollOffset(0.0f),
      addToOrderHovered(false),
      createSuborderHovered(false),
      proceedToCheckoutHovered(false),
      backHovered(false),
      restartHovered(false),
      confirmNameHovered(false),
      cancelNameHovered(false) {

    InitializeLayout();
    InitializeButtons();
}

OrderCreationScreen::~OrderCreationScreen() {
    CleanupOrderHierarchy();
}

void OrderCreationScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    // Split screen: 40% left (cart), 60% right (orders)
    int dividerX = screenWidth * 0.4f;

    leftPanel = Rectangle{
        10,
        100,
        static_cast<float>(dividerX - 20),
        static_cast<float>(screenHeight - 220)
    };

    rightPanel = Rectangle{
        static_cast<float>(dividerX + 10),
        100,
        static_cast<float>(screenWidth - dividerX - 20),
        static_cast<float>(screenHeight - 220)
    };
}

void OrderCreationScreen::InitializeButtons() {
    int buttonWidth = 180;
    int buttonHeight = 45;
    int buttonSpacing = 15;
    int bottomY = screenHeight - 110;

    // Bottom row - centered horizontally, evenly spaced
    int totalWidth = (buttonWidth * 4) + (buttonSpacing * 3);
    int startX = (screenWidth - totalWidth) / 2;

    // Row 1: Main action buttons (left to right)
    addToOrderButton = Rectangle{
        static_cast<float>(startX),
        static_cast<float>(bottomY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    createSuborderButton = Rectangle{
        static_cast<float>(startX + buttonWidth + buttonSpacing),
        static_cast<float>(bottomY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    restartButton = Rectangle{
        static_cast<float>(startX + (buttonWidth + buttonSpacing) * 2),
        static_cast<float>(bottomY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    backButton = Rectangle{
        static_cast<float>(startX + (buttonWidth + buttonSpacing) * 3),
        static_cast<float>(bottomY),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    // Row 2: Proceed to Checkout (centered, larger)
    int checkoutWidth = 270;
    proceedToCheckoutButton = Rectangle{
        static_cast<float>(screenWidth / 2 - checkoutWidth / 2),
        static_cast<float>(bottomY + buttonHeight + buttonSpacing),
        static_cast<float>(checkoutWidth),
        static_cast<float>(buttonHeight + 5)
    };

    // Suborder name input modal (centered)
    int modalWidth = 400;
    int modalHeight = 200;
    nameInputBox = Rectangle{
        static_cast<float>(screenWidth / 2 - modalWidth / 2),
        static_cast<float>(screenHeight / 2 - modalHeight / 2),
        static_cast<float>(modalWidth),
        40
    };

    confirmNameButton = Rectangle{
        nameInputBox.x,
        nameInputBox.y + 60,
        180,
        50
    };

    cancelNameButton = Rectangle{
        nameInputBox.x + 220,
        nameInputBox.y + 60,
        180,
        50
    };
}

void OrderCreationScreen::InitializeCartTrackers() {
    cartTrackers.clear();
    Customer* customer = manager->GetCustomer();
    if (customer != nullptr) {
        int cartSize = customer->getCartSize();
        for (int i = 0; i < cartSize; i++) {
            cartTrackers.push_back(CartItemTracker(i));
        }
    }
}

void OrderCreationScreen::InitializeRootOrder() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;

    // Create root order with customer name
    std::string rootName = "Customer " + customer->getName() + " Order";
    ConcreteOrder* rootOrder = new ConcreteOrder(rootName);

    // Create root node
    rootOrderNode = new OrderNode(rootOrder, nullptr, 0);
    selectedOrderNode = rootOrderNode;

    std::cout << "[OrderCreationScreen] Initialized root order: " << rootName << std::endl;
}

void OrderCreationScreen::Reset() {
    std::cout << "[OrderCreationScreen] Resetting screen state..." << std::endl;

    // Cleanup old hierarchy
    CleanupOrderHierarchy();

    // Reset state
    selectedCartIndices.clear();
    isCreatingSuborder = false;
    suborderNameInput.clear();
    cartScrollOffset = 0.0f;
    orderScrollOffset = 0.0f;

    // Initialize fresh state
    InitializeCartTrackers();
    InitializeRootOrder();

    std::cout << "[OrderCreationScreen] Reset complete" << std::endl;
}

void OrderCreationScreen::CleanupOrderHierarchy() {
    if (rootOrderNode != nullptr) {
        // Note: We don't delete the ConcreteOrder objects here because
        // they'll be managed by the Customer or FinalOrder
        // We only delete the visual hierarchy nodes
        delete rootOrderNode;
        rootOrderNode = nullptr;
        selectedOrderNode = nullptr;
    }
}

void OrderCreationScreen::Update() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || customer->getCartSize() == 0) {
        manager->SwitchScreen(GameScreen::SALES_FLOOR);
        return;
    }

    if (isCreatingSuborder) {
        UpdateTextInput();
    } else {
        UpdateCartSelection();
        UpdateOrderSelection();
        UpdateButtons();
    }
}

void OrderCreationScreen::UpdateCartSelection() {
    Vector2 mousePos = GetMousePosition();

    if (!CheckCollisionPointRec(mousePos, leftPanel)) {
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // Calculate which cart item was clicked
        int itemHeight = 60;
        int yOffset = leftPanel.y + 40 - cartScrollOffset;

        for (size_t i = 0; i < cartTrackers.size(); i++) {
            Rectangle itemRect = Rectangle{
                leftPanel.x + 10,
                static_cast<float>(yOffset + i * itemHeight),
                leftPanel.width - 20,
                static_cast<float>(itemHeight - 5)
            };

            if (CheckCollisionPointRec(mousePos, itemRect)) {
                // Toggle selection
                if (selectedCartIndices.count(i) > 0) {
                    selectedCartIndices.erase(i);
                    std::cout << "[OrderCreationScreen] Deselected cart item " << i << std::endl;
                } else {
                    selectedCartIndices.insert(i);
                    std::cout << "[OrderCreationScreen] Selected cart item " << i << std::endl;
                }
                break;
            }
        }
    }

    // Handle scroll wheel for cart
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0 && CheckCollisionPointRec(mousePos, leftPanel)) {
        cartScrollOffset -= wheelMove * 20;
        if (cartScrollOffset < 0) cartScrollOffset = 0;
    }
}

void OrderCreationScreen::UpdateOrderSelection() {
    Vector2 mousePos = GetMousePosition();

    if (!CheckCollisionPointRec(mousePos, rightPanel)) {
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // Calculate which order was clicked
        // This is simplified - in DrawOrderNode we'll need to track clickable areas
        // For now, just handle scroll
    }

    // Handle scroll wheel for orders
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0 && CheckCollisionPointRec(mousePos, rightPanel)) {
        orderScrollOffset -= wheelMove * 20;
        if (orderScrollOffset < 0) orderScrollOffset = 0;
    }
}

void OrderCreationScreen::UpdateTextInput() {
    int key = GetCharPressed();

    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (suborderNameInput.length() < maxSuborderNameLength)) {
            suborderNameInput += (char)key;
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !suborderNameInput.empty()) {
        suborderNameInput.pop_back();
    }

    if (IsKeyPressed(KEY_ENTER) && !suborderNameInput.empty()) {
        HandleConfirmName();
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        HandleCancelName();
    }

    // Check button clicks
    Vector2 mousePos = GetMousePosition();
    confirmNameHovered = CheckCollisionPointRec(mousePos, confirmNameButton);
    cancelNameHovered = CheckCollisionPointRec(mousePos, cancelNameButton);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (confirmNameHovered && !suborderNameInput.empty()) {
            HandleConfirmName();
        } else if (cancelNameHovered) {
            HandleCancelName();
        }
    }
}

void OrderCreationScreen::UpdateButtons() {
    Vector2 mousePos = GetMousePosition();

    addToOrderHovered = CheckCollisionPointRec(mousePos, addToOrderButton);
    createSuborderHovered = CheckCollisionPointRec(mousePos, createSuborderButton);
    proceedToCheckoutHovered = CheckCollisionPointRec(mousePos, proceedToCheckoutButton) && AllCartItemsAdded();
    backHovered = CheckCollisionPointRec(mousePos, backButton);
    restartHovered = CheckCollisionPointRec(mousePos, restartButton);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (addToOrderHovered) {
            HandleAddToOrder();
        } else if (createSuborderHovered) {
            HandleCreateSuborder();
        } else if (proceedToCheckoutHovered && AllCartItemsAdded()) {
            HandleProceedToCheckout();
        } else if (backHovered) {
            HandleBack();
        } else if (restartHovered) {
            HandleRestart();
        }
    }
}

void OrderCreationScreen::HandleAddToOrder() {
    if (selectedCartIndices.empty()) {
        std::cout << "[OrderCreationScreen] No cart items selected" << std::endl;
        return;
    }

    if (selectedOrderNode == nullptr) {
        std::cout << "[OrderCreationScreen] No order selected" << std::endl;
        return;
    }

    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;

    int addedCount = 0;
    std::vector<int> indicesToDeselect;

    // Add all selected items that haven't been added yet
    for (int index : selectedCartIndices) {
        if (index < 0 || index >= (int)cartTrackers.size()) {
            continue;
        }

        if (cartTrackers[index].isAddedToOrder) {
            std::cout << "[OrderCreationScreen] Cart item " << index << " already added, skipping" << std::endl;
            continue;
        }

        Plant* plant = customer->getPlantFromCart(index);
        if (plant == nullptr) continue;

        // Create a Leaf with ownsPlant=false (cart still owns the plant)
        Leaf* leaf = new Leaf(plant, false);

        // Add to the selected order
        selectedOrderNode->order->add(leaf);
        selectedOrderNode->leafCartIndices.push_back(index);

        // Mark as added
        cartTrackers[index].isAddedToOrder = true;
        indicesToDeselect.push_back(index);
        addedCount++;

        std::cout << "[OrderCreationScreen] Added cart item " << index
                  << " (" << plant->getName() << ") to order \""
                  << selectedOrderNode->order->getName() << "\"" << std::endl;
    }

    // Deselect items that were added
    for (int index : indicesToDeselect) {
        selectedCartIndices.erase(index);
    }

    std::cout << "[OrderCreationScreen] Added " << addedCount << " items to order" << std::endl;
}

void OrderCreationScreen::HandleCreateSuborder() {
    if (selectedOrderNode == nullptr) {
        std::cout << "[OrderCreationScreen] No order selected to create suborder in" << std::endl;
        return;
    }

    // Open text input modal
    isCreatingSuborder = true;
    suborderNameInput.clear();
    std::cout << "[OrderCreationScreen] Opening suborder creation modal" << std::endl;
}

void OrderCreationScreen::HandleConfirmName() {
    if (suborderNameInput.empty() || selectedOrderNode == nullptr) {
        return;
    }

    // Create new suborder
    ConcreteOrder* suborder = new ConcreteOrder(suborderNameInput);

    // Add to parent order
    selectedOrderNode->order->add(suborder);

    // Create visual node
    OrderNode* subNode = new OrderNode(suborder, selectedOrderNode, selectedOrderNode->depth + 1);
    selectedOrderNode->children.push_back(subNode);

    std::cout << "[OrderCreationScreen] Created suborder \"" << suborderNameInput
              << "\" under \"" << selectedOrderNode->order->getName() << "\"" << std::endl;

    // Close modal
    isCreatingSuborder = false;
    suborderNameInput.clear();

    // Select the new suborder
    selectedOrderNode = subNode;
}

void OrderCreationScreen::HandleCancelName() {
    isCreatingSuborder = false;
    suborderNameInput.clear();
    std::cout << "[OrderCreationScreen] Cancelled suborder creation" << std::endl;
}

void OrderCreationScreen::HandleProceedToCheckout() {
    if (!AllCartItemsAdded()) {
        std::cout << "[OrderCreationScreen] Cannot proceed - not all items added to orders" << std::endl;
        return;
    }

    Customer* customer = manager->GetCustomer();
    if (customer == nullptr || rootOrderNode == nullptr) return;

    // Create FinalOrder with the root order
    FinalOrder* finalOrder = new FinalOrder(customer->getName());
    finalOrder->addOrder(rootOrderNode->order);

    // Store in manager
    manager->SetFinalOrder(finalOrder);

    // Don't cleanup the order hierarchy yet - FinalOrder now owns it
    // Just cleanup our visual tracking
    rootOrderNode = nullptr;
    selectedOrderNode = nullptr;

    std::cout << "[OrderCreationScreen] Created FinalOrder, proceeding to checkout" << std::endl;
    std::cout << "[OrderCreationScreen] Total: R" << finalOrder->calculateTotalPrice() << std::endl;

    // Navigate to checkout screen
    manager->SwitchScreen(GameScreen::CHECKOUT);
}

void OrderCreationScreen::HandleBack() {
    std::cout << "[OrderCreationScreen] Returning to sales floor" << std::endl;
    manager->SwitchScreen(GameScreen::SALES_FLOOR);
}

void OrderCreationScreen::HandleRestart() {
    std::cout << "[OrderCreationScreen] Restarting order creation" << std::endl;
    Reset();
}

void OrderCreationScreen::Draw() {
    Color bgColor = manager->IsAlternativeColors()
        ? Color{30, 30, 35, 255}     // Dark mode
        : Color{216, 228, 220, 255}; // Soft sage green
    ClearBackground(bgColor);

    // Draw header
    const char* header = "CREATE YOUR ORDER";
    int headerSize = 36;
    int headerWidth = MeasureText(header, headerSize);
    Color headerColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawText(header, screenWidth / 2 - headerWidth / 2, 30, headerSize, headerColor);

    // Draw instructions
    const char* instruction = "Click items to select/deselect (multi-select enabled) - Add all selected to order";
    int instrWidth = MeasureText(instruction, 16);
    Color instrColor = manager->IsAlternativeColors()
        ? Color{180, 200, 190, 255}  // Light grey-green
        : Color{120, 140, 125, 255}; // Medium sage
    DrawText(instruction, screenWidth / 2 - instrWidth / 2, 75, 16, instrColor);

    if (isCreatingSuborder) {
        // Draw modal overlay
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});
        DrawSuborderCreationModal();
    } else {
        DrawLeftPanel();
        DrawRightPanel();
        DrawButtons();
    }
}

void OrderCreationScreen::DrawLeftPanel() {
    // Panel background
    Color panelBg = manager->IsAlternativeColors()
        ? Color{45, 45, 50, 255}     // Dark panel
        : Color{210, 210, 210, 255}; // Light grey
    DrawRectangleRec(leftPanel, panelBg);

    Color panelBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint border
        : Color{120, 120, 120, 255}; // Dark grey border
    DrawRectangleLinesEx(leftPanel, 2, panelBorder);

    // Panel title
    const char* title = "CART ITEMS";
    int titleWidth = MeasureText(title, 24);
    Color titleColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawText(title, leftPanel.x + (leftPanel.width - titleWidth) / 2, leftPanel.y + 10, 24, titleColor);

    DrawCartList();
}

void OrderCreationScreen::DrawCartList() {
    Customer* customer = manager->GetCustomer();
    if (customer == nullptr) return;

    int itemHeight = 60;
    int yOffset = leftPanel.y + 40 - cartScrollOffset;

    for (size_t i = 0; i < cartTrackers.size(); i++) {
        Plant* plant = customer->getPlantFromCart(i);
        if (plant == nullptr) continue;

        Rectangle itemRect = Rectangle{
            leftPanel.x + 10,
            static_cast<float>(yOffset + i * itemHeight),
            leftPanel.width - 20,
            static_cast<float>(itemHeight - 5)
        };

        // Skip if not visible
        if (itemRect.y + itemRect.height < leftPanel.y || itemRect.y > leftPanel.y + leftPanel.height) {
            continue;
        }

        // Determine color based on state
        Color bgColor;
        Color borderColor;
        if (cartTrackers[i].isAddedToOrder) {
            bgColor = manager->IsAlternativeColors()
                ? Color{50, 80, 60, 255}     // Dark green - added
                : Color{200, 235, 200, 255}; // Light green - added
            borderColor = Color{120, 165, 120, 255}; // Soft green (same in both modes)
        } else if (selectedCartIndices.count(i) > 0) {
            bgColor = manager->IsAlternativeColors()
                ? Color{80, 90, 60, 255}     // Dark yellow-ish - selected
                : Color{255, 247, 204, 255}; // Soft butter yellow - selected
            borderColor = manager->IsAlternativeColors()
                ? Color{255, 180, 160, 255}  // Light terracotta
                : Color{235, 186, 170, 255}; // Warm terracotta
        } else {
            bgColor = manager->IsAlternativeColors()
                ? Color{55, 60, 55, 255}     // Dark grey - normal
                : Color{245, 250, 247, 255}; // Very light sage - normal
            borderColor = manager->IsAlternativeColors()
                ? Color{100, 110, 100, 255}  // Medium grey border
                : Color{200, 210, 205, 255}; // Light sage
        }

        DrawRectangleRec(itemRect, bgColor);
        DrawRectangleLinesEx(itemRect, 2, borderColor);

        // Draw plant name
        std::string displayName = plant->getName();
        if (displayName.length() > 15) {
            displayName = displayName.substr(0, 12) + "...";
        }

        Color nameColor = manager->IsAlternativeColors()
            ? Color{200, 200, 200, 255}  // Light grey
            : Color{85, 107, 95, 255};   // Dark forest green
        DrawText(displayName.c_str(), itemRect.x + 5, itemRect.y + 5, 18, nameColor);

        // Draw price
        std::ostringstream priceStream;
        priceStream << "R" << std::fixed << std::setprecision(2) << plant->getPrice();
        Color priceColor = manager->IsAlternativeColors()
            ? Color{255, 180, 160, 255}  // Light terracotta
            : Color{235, 186, 170, 255}; // Warm terracotta
        DrawText(priceStream.str().c_str(), itemRect.x + 5, itemRect.y + 28, 16, priceColor);

        // Draw status
        if (cartTrackers[i].isAddedToOrder) {
            Color statusColor = manager->IsAlternativeColors()
                ? Color{160, 200, 170, 255}  // Light green-grey
                : Color{120, 140, 125, 255}; // Medium sage
            DrawText("ADDED", itemRect.x + itemRect.width - 65, itemRect.y + 20, 14, statusColor);
        }
    }
}

void OrderCreationScreen::DrawRightPanel() {
    // Panel background
    Color panelBg = manager->IsAlternativeColors()
        ? Color{45, 45, 50, 255}     // Dark panel
        : Color{210, 210, 210, 255}; // Light grey
    DrawRectangleRec(rightPanel, panelBg);

    Color panelBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint border
        : Color{120, 120, 120, 255}; // Dark grey border
    DrawRectangleLinesEx(rightPanel, 2, panelBorder);

    // Panel title
    const char* title = "ORDER HIERARCHY";
    int titleWidth = MeasureText(title, 24);
    Color titleColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawText(title, rightPanel.x + (rightPanel.width - titleWidth) / 2, rightPanel.y + 10, 24, titleColor);

    DrawOrderHierarchy();
}

void OrderCreationScreen::DrawOrderHierarchy() {
    if (rootOrderNode == nullptr) return;

    int yPos = rightPanel.y + 45 - orderScrollOffset;
    DrawOrderNode(rootOrderNode, yPos);
}

void OrderCreationScreen::DrawOrderNode(OrderNode* node, int& yPos) {
    if (node == nullptr) return;

    int indentSize = 30;
    int indent = node->depth * indentSize;
    int nodeHeight = 50;

    Rectangle nodeRect = Rectangle{
        rightPanel.x + 10 + indent,
        static_cast<float>(yPos),
        rightPanel.width - 20 - indent,
        static_cast<float>(nodeHeight)
    };

    // Skip if not visible
    if (nodeRect.y + nodeRect.height < rightPanel.y || nodeRect.y > rightPanel.y + rightPanel.height) {
        yPos += nodeHeight + 5;

        // Still need to process children for height calculation
        if (node->isExpanded) {
            for (OrderNode* child : node->children) {
                DrawOrderNode(child, yPos);
            }
        }
        return;
    }

    // Check if clicked
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, nodeRect)) {
        selectedOrderNode = node;
        std::cout << "[OrderCreationScreen] Selected order: " << node->order->getName() << std::endl;
    }

    // Determine color
    Color bgNormal = manager->IsAlternativeColors()
        ? Color{55, 60, 55, 255}     // Dark grey
        : Color{245, 250, 247, 255}; // Very light sage
    Color bgSelected = manager->IsAlternativeColors()
        ? Color{80, 90, 60, 255}     // Dark yellow-ish
        : Color{255, 247, 204, 255}; // Soft butter yellow
    Color bgColor = (selectedOrderNode == node) ? bgSelected : bgNormal;
    DrawRectangleRec(nodeRect, bgColor);

    Color borderNormal = manager->IsAlternativeColors()
        ? Color{100, 110, 100, 255}  // Medium grey
        : Color{200, 210, 205, 255}; // Light sage
    Color borderSelected = manager->IsAlternativeColors()
        ? Color{255, 180, 160, 255}  // Light terracotta
        : Color{235, 186, 170, 255}; // Warm terracotta
    DrawRectangleLinesEx(nodeRect, 2, (selectedOrderNode == node) ? borderSelected : borderNormal);

    // Draw order name
    std::string displayName = node->order->getName();
    if (displayName.length() > 25) {
        displayName = displayName.substr(0, 22) + "...";
    }
    Color nameColor = manager->IsAlternativeColors()
        ? Color{200, 200, 200, 255}  // Light grey
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawText(displayName.c_str(), nodeRect.x + 5, nodeRect.y + 5, 18, nameColor);

    // Draw item count
    int leafCount = node->leafCartIndices.size();
    int childCount = node->children.size();
    std::ostringstream infoStream;
    infoStream << "Items: " << leafCount << " | Suborders: " << childCount;
    Color infoColor = manager->IsAlternativeColors()
        ? Color{160, 180, 170, 255}  // Medium grey-green
        : Color{120, 140, 125, 255}; // Medium sage
    DrawText(infoStream.str().c_str(), nodeRect.x + 5, nodeRect.y + 28, 14, infoColor);

    yPos += nodeHeight + 5;

    // Draw children if expanded
    if (node->isExpanded) {
        for (OrderNode* child : node->children) {
            DrawOrderNode(child, yPos);
        }
    }
}

void OrderCreationScreen::DrawButtons() {
    int fontSize = 18;

    Color buttonBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    Color disabledColor = manager->IsAlternativeColors()
        ? Color{60, 60, 65, 255}     // Dark disabled
        : Color{220, 220, 220, 255}; // Light grey disabled
    Color activeText = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : Color{85, 107, 95, 255};   // Dark forest green
    Color disabledText = manager->IsAlternativeColors()
        ? Color{100, 100, 100, 255}  // Dark grey
        : Color{150, 150, 150, 255}; // Medium grey

    // Add to Order button
    bool canAdd = false;
    if (selectedOrderNode != nullptr && !selectedCartIndices.empty()) {
        for (int index : selectedCartIndices) {
            if (index >= 0 && index < (int)cartTrackers.size() && !cartTrackers[index].isAddedToOrder) {
                canAdd = true;
                break;
            }
        }
    }
    Color addIdle = manager->IsAlternativeColors()
        ? Color{50, 80, 60, 255}     // Dark green
        : Color{120, 165, 120, 255}; // Soft green
    Color addHover = manager->IsAlternativeColors()
        ? Color{65, 100, 75, 255}    // Lighter green
        : Color{140, 200, 150, 255}; // Soft green hover
    Color addColor = canAdd ? (addToOrderHovered ? addHover : addIdle) : disabledColor;
    DrawRectangleRec(addToOrderButton, addColor);
    DrawRectangleLinesEx(addToOrderButton, 2, buttonBorder);

    // Show count of selected items in button text
    std::string addText = "ADD TO ORDER";
    if (!selectedCartIndices.empty()) {
        addText += " (" + std::to_string(selectedCartIndices.size()) + ")";
    }
    int addTextWidth = MeasureText(addText.c_str(), fontSize);
    DrawText(addText.c_str(),
             addToOrderButton.x + (addToOrderButton.width - addTextWidth) / 2,
             addToOrderButton.y + (addToOrderButton.height - fontSize) / 2,
             fontSize,
             canAdd ? activeText : disabledText);

    // Create Suborder button
    bool canCreateSub = selectedOrderNode != nullptr;
    Color subIdle = manager->IsAlternativeColors()
        ? Color{70, 60, 80, 255}     // Dark lavender
        : Color{230, 224, 237, 255}; // Soft lavender
    Color subHover = manager->IsAlternativeColors()
        ? Color{85, 75, 95, 255}     // Lighter lavender
        : Color{215, 195, 220, 255}; // Lavender hover
    Color subColor = canCreateSub ? (createSuborderHovered ? subHover : subIdle) : disabledColor;
    DrawRectangleRec(createSuborderButton, subColor);
    DrawRectangleLinesEx(createSuborderButton, 2, buttonBorder);
    const char* subText = "SUBORDER";
    int subTextWidth = MeasureText(subText, fontSize);
    DrawText(subText,
             createSuborderButton.x + (createSuborderButton.width - subTextWidth) / 2,
             createSuborderButton.y + (createSuborderButton.height - fontSize) / 2,
             fontSize,
             canCreateSub ? activeText : disabledText);

    // Restart button
    Color restartIdle = manager->IsAlternativeColors()
        ? Color{80, 50, 50, 255}     // Dark red
        : Color{245, 215, 220, 255}; // Soft rose
    Color restartHover = manager->IsAlternativeColors()
        ? Color{100, 65, 65, 255}    // Lighter red
        : Color{255, 200, 195, 255}; // Soft coral hover
    Color restartColor = restartHovered ? restartHover : restartIdle;
    DrawRectangleRec(restartButton, restartColor);
    DrawRectangleLinesEx(restartButton, 2, buttonBorder);
    const char* restartText = "RESTART";
    int restartTextWidth = MeasureText(restartText, fontSize);
    DrawText(restartText,
             restartButton.x + (restartButton.width - restartTextWidth) / 2,
             restartButton.y + (restartButton.height - fontSize) / 2,
             fontSize,
             activeText);

    // Back button (returns to sales floor)
    Color backIdle = manager->IsAlternativeColors()
        ? Color{60, 55, 60, 255}     // Dark grey
        : Color{245, 240, 242, 255}; // Very light grey
    Color backHover = manager->IsAlternativeColors()
        ? Color{75, 70, 75, 255}     // Lighter grey
        : Color{235, 225, 230, 255}; // Soft rose hover
    Color backColor = backHovered ? backHover : backIdle;
    DrawRectangleRec(backButton, backColor);
    DrawRectangleLinesEx(backButton, 2, buttonBorder);
    const char* backText = "BACK";
    int backTextWidth = MeasureText(backText, fontSize);
    DrawText(backText,
             backButton.x + (backButton.width - backTextWidth) / 2,
             backButton.y + (backButton.height - fontSize) / 2,
             fontSize,
             activeText);

    // Proceed to Checkout button (larger, bottom row)
    bool allAdded = AllCartItemsAdded();
    int checkoutFontSize = 20;
    Color checkoutIdle = manager->IsAlternativeColors()
        ? Color{100, 80, 60, 255}    // Dark peachy
        : Color{255, 236, 214, 255}; // Peachy cream
    Color checkoutHover = manager->IsAlternativeColors()
        ? Color{120, 100, 75, 255}   // Lighter peachy
        : Color{250, 220, 170, 255}; // Peachy cream hover
    Color checkoutColor = allAdded ? (proceedToCheckoutHovered ? checkoutHover : checkoutIdle) : disabledColor;
    DrawRectangleRec(proceedToCheckoutButton, checkoutColor);

    Color checkoutBorderActive = manager->IsAlternativeColors()
        ? Color{255, 180, 160, 255}  // Light terracotta
        : Color{235, 186, 170, 255}; // Warm terracotta
    Color checkoutBorderDisabled = manager->IsAlternativeColors()
        ? Color{80, 80, 85, 255}     // Dark grey
        : Color{180, 180, 180, 255}; // Light grey
    DrawRectangleLinesEx(proceedToCheckoutButton, 3, allAdded ? checkoutBorderActive : checkoutBorderDisabled);

    const char* checkoutText = "PROCEED TO CHECKOUT";
    int checkoutTextWidth = MeasureText(checkoutText, checkoutFontSize);
    DrawText(checkoutText,
             proceedToCheckoutButton.x + (proceedToCheckoutButton.width - checkoutTextWidth) / 2,
             proceedToCheckoutButton.y + (proceedToCheckoutButton.height - checkoutFontSize) / 2,
             checkoutFontSize,
             allAdded ? activeText : disabledText);
}

void OrderCreationScreen::DrawSuborderCreationModal() {
    int modalWidth = 400;
    int modalHeight = 200;
    Rectangle modal = Rectangle{
        static_cast<float>(screenWidth / 2 - modalWidth / 2),
        static_cast<float>(screenHeight / 2 - modalHeight / 2),
        static_cast<float>(modalWidth),
        static_cast<float>(modalHeight)
    };

    // Modal background
    Color modalBg = manager->IsAlternativeColors()
        ? Color{45, 45, 50, 255}     // Dark lavender
        : Color{230, 224, 237, 255}; // Soft lavender
    DrawRectangleRec(modal, modalBg);

    Color modalBorder = manager->IsAlternativeColors()
        ? Color{255, 180, 160, 255}  // Light terracotta
        : Color{235, 186, 170, 255}; // Warm terracotta
    DrawRectangleLinesEx(modal, 3, modalBorder);

    // Title
    const char* title = "Create Suborder";
    int titleWidth = MeasureText(title, 24);
    Color titleColor = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawText(title, modal.x + (modal.width - titleWidth) / 2, modal.y + 10, 24, titleColor);

    // Instruction
    const char* instr = "Enter suborder name:";
    Color instrColor = manager->IsAlternativeColors()
        ? Color{180, 200, 190, 255}  // Light grey-green
        : Color{120, 140, 125, 255}; // Medium sage
    DrawText(instr, nameInputBox.x, nameInputBox.y - 25, 18, instrColor);

    // Text input box
    Color inputBg = manager->IsAlternativeColors()
        ? Color{60, 65, 60, 255}     // Dark input
        : Color{245, 250, 247, 255}; // Very light sage
    DrawRectangleRec(nameInputBox, inputBg);
    DrawRectangleLinesEx(nameInputBox, 2, modalBorder);

    Color inputText = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : Color{85, 107, 95, 255};   // Dark forest green
    DrawText(suborderNameInput.c_str(), nameInputBox.x + 5, nameInputBox.y + 10, 20, inputText);

    // Cursor
    if ((int)(GetTime() * 2) % 2 == 0) {
        int textWidth = MeasureText(suborderNameInput.c_str(), 20);
        DrawText("_", nameInputBox.x + 5 + textWidth, nameInputBox.y + 10, 20, inputText);
    }

    Color buttonBorder = manager->IsAlternativeColors()
        ? Color{150, 220, 180, 255}  // Light mint
        : Color{85, 107, 95, 255};   // Dark forest green
    Color disabledColor = manager->IsAlternativeColors()
        ? Color{60, 60, 65, 255}     // Dark disabled
        : Color{220, 220, 220, 255}; // Light grey disabled
    Color activeText = manager->IsAlternativeColors()
        ? Color{240, 240, 240, 255}  // Light text
        : Color{85, 107, 95, 255};   // Dark forest green
    Color disabledText = manager->IsAlternativeColors()
        ? Color{100, 100, 100, 255}  // Dark grey
        : Color{150, 150, 150, 255}; // Medium grey

    // Confirm button
    Color confirmIdle = manager->IsAlternativeColors()
        ? Color{50, 80, 60, 255}     // Dark green
        : Color{120, 165, 120, 255}; // Soft green
    Color confirmHover = manager->IsAlternativeColors()
        ? Color{65, 100, 75, 255}    // Lighter green
        : Color{140, 200, 150, 255}; // Soft green hover
    Color confirmColor = !suborderNameInput.empty() ? (confirmNameHovered ? confirmHover : confirmIdle) : disabledColor;
    DrawRectangleRec(confirmNameButton, confirmColor);
    DrawRectangleLinesEx(confirmNameButton, 2, buttonBorder);
    const char* confirmText = "CONFIRM";
    int confirmTextWidth = MeasureText(confirmText, 20);
    DrawText(confirmText,
             confirmNameButton.x + (confirmNameButton.width - confirmTextWidth) / 2,
             confirmNameButton.y + (confirmNameButton.height - 20) / 2,
             20,
             !suborderNameInput.empty() ? activeText : disabledText);

    // Cancel button
    Color cancelIdle = manager->IsAlternativeColors()
        ? Color{60, 55, 60, 255}     // Dark grey
        : Color{245, 240, 242, 255}; // Very light grey
    Color cancelHover = manager->IsAlternativeColors()
        ? Color{75, 70, 75, 255}     // Lighter grey
        : Color{235, 225, 230, 255}; // Soft rose hover
    Color cancelColor = cancelNameHovered ? cancelHover : cancelIdle;
    DrawRectangleRec(cancelNameButton, cancelColor);
    DrawRectangleLinesEx(cancelNameButton, 2, buttonBorder);
    const char* cancelText = "CANCEL";
    int cancelTextWidth = MeasureText(cancelText, 20);
    DrawText(cancelText,
             cancelNameButton.x + (cancelNameButton.width - cancelTextWidth) / 2,
             cancelNameButton.y + (cancelNameButton.height - 20) / 2,
             20,
             activeText);
}

bool OrderCreationScreen::AllCartItemsAdded() const {
    for (const CartItemTracker& tracker : cartTrackers) {
        if (!tracker.isAddedToOrder) {
            return false;
        }
    }
    return true;
}

int OrderCreationScreen::CountOrderNodes(OrderNode* node) const {
    if (node == nullptr) return 0;

    int count = 1;
    for (OrderNode* child : node->children) {
        count += CountOrderNodes(child);
    }
    return count;
}
