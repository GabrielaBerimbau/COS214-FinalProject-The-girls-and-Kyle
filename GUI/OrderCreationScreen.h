#ifndef ORDER_CREATION_SCREEN_H
#define ORDER_CREATION_SCREEN_H

#include "raylib.h"
#include <string>
#include <vector>
#include <set>

class ScreenManager;
class Order;
class ConcreteOrder;

/**
 * @brief Structure to track cart items that have been added to orders
 */
struct CartItemTracker {
    int cartIndex;
    bool isAddedToOrder;

    CartItemTracker(int idx) : cartIndex(idx), isAddedToOrder(false) {}
};

/**
 * @brief Structure to represent the visual hierarchy of orders
 */
struct OrderNode {
    ConcreteOrder* order;
    OrderNode* parent;
    std::vector<OrderNode*> children;
    int depth;
    bool isExpanded;
    std::vector<int> leafCartIndices; // Cart indices of leaves directly in this order

    OrderNode(ConcreteOrder* ord, OrderNode* par = nullptr, int d = 0)
        : order(ord), parent(par), depth(d), isExpanded(true) {}

    ~OrderNode() {
        for (OrderNode* child : children) {
            delete child;
        }
        children.clear();
    }
};

/**
 * @brief Screen for building composite orders from cart items
 *
 * This screen allows users to:
 * - Select cart items and add them to orders
 * - Create nested suborders within orders
 * - View the order hierarchy visually
 * - Proceed to checkout when all items are added
 */
class OrderCreationScreen {
private:
    ScreenManager* manager;

    // Layout dimensions
    int screenWidth;
    int screenHeight;

    // Split screen layout
    Rectangle leftPanel;   // Cart items list
    Rectangle rightPanel;  // Order hierarchy

    // Cart tracking
    std::vector<CartItemTracker> cartTrackers;
    std::set<int> selectedCartIndices;  // Changed to support multi-select

    // Order hierarchy tracking
    OrderNode* rootOrderNode;
    OrderNode* selectedOrderNode;

    // UI state
    bool isCreatingSuborder;
    std::string suborderNameInput;
    int maxSuborderNameLength;

    // Scroll state
    float cartScrollOffset;
    float orderScrollOffset;

    // UI Buttons
    Rectangle addToOrderButton;
    Rectangle createSuborderButton;
    Rectangle proceedToCheckoutButton;
    Rectangle backButton;
    Rectangle restartButton;

    // Text input box for suborder creation
    Rectangle nameInputBox;
    Rectangle confirmNameButton;
    Rectangle cancelNameButton;

    // Button hover states
    bool addToOrderHovered;
    bool createSuborderHovered;
    bool proceedToCheckoutHovered;
    bool backHovered;
    bool restartHovered;
    bool confirmNameHovered;
    bool cancelNameHovered;

    // Helper methods
    void InitializeLayout();
    void InitializeButtons();
    void InitializeCartTrackers();
    void InitializeRootOrder();

    void UpdateButtons();
    void UpdateCartSelection();
    void UpdateOrderSelection();
    void UpdateTextInput();

    void HandleAddToOrder();
    void HandleCreateSuborder();
    void HandleProceedToCheckout();
    void HandleBack();
    void HandleRestart();
    void HandleConfirmName();
    void HandleCancelName();

    void DrawLeftPanel();
    void DrawRightPanel();
    void DrawCartList();
    void DrawOrderHierarchy();
    void DrawOrderNode(OrderNode* node, int& yPos);
    void DrawButtons();
    void DrawSuborderCreationModal();

    bool AllCartItemsAdded() const;
    int CountOrderNodes(OrderNode* node) const;
    int CalculateOrderHierarchyHeight(OrderNode* node) const;

    void CleanupOrderHierarchy();

public:
    OrderCreationScreen(ScreenManager* mgr);
    ~OrderCreationScreen();

    void Update();
    void Draw();
    void Reset(); // Reset screen state when entering
};

#endif // ORDER_CREATION_SCREEN_H
