#include "include/Customer.h"
#include "include/Plant.h"
#include "include/Request.h"
#include "include/NurseryMediator.h"
#include "include/ConcreteOrder.h"
#include "include/FinalOrder.h"
#include "include/Leaf.h"
#include "include/RibbonDecorator.h"
#include "include/GiftWrapDecorator.h"
#include "include/DecorativePotDecorator.h"
#include "include/StaffMembers.h"
#include <algorithm>
#include <iostream>

Customer::Customer(NurseryMediator* m, const std::string& name, 
                   const std::string& id, double initialBudget)
    : Person(m, name, id), budget(initialBudget), currentRequest(nullptr), currentOrder(nullptr) {
    std::cout << "[Customer] " << name << " created with budget R" << initialBudget << "\n";
}

Customer::~Customer() {
    // Clear cart - delete all plants we own
    for (Plant* plant : cart) {
        delete plant;
    }
    cart.clear();
    
    if (currentRequest != nullptr) {
        delete currentRequest;
        currentRequest = nullptr;
    }
    
    if (currentOrder != nullptr) {
        delete currentOrder;
        currentOrder = nullptr;
    }
}

// ============ CART MANAGEMENT (via Mediator) ============

bool Customer::addPlantFromSalesFloor(const std::string& plantName) {
    if (mediator == nullptr) {
        std::cout << "[Customer] No mediator available.\n";
        return false;
    }
    
    std::cout << "[Customer] " << getName() << " requesting plant: " << plantName << "\n";
    
    bool success = mediator->transferPlantToCustomer(plantName, this);
    
    if (!success) {
        std::cout << "[Customer] " << mediator->plantUnavailable() << "\n";
    }
    
    return success;
}

bool Customer::addPlantFromSalesFloorPosition(int row, int col) {
    if (mediator == nullptr) {
        std::cout << "[Customer] No mediator available.\n";
        return false;
    }
    
    std::cout << "[Customer] " << getName() << " requesting plant at position (" 
              << row << "," << col << ")\n";
    
    bool success = mediator->transferPlantFromPosition(row, col, this);
    
    return success;
}

bool Customer::returnPlantToSalesFloor(int cartIndex) {
    if (cartIndex < 0 || cartIndex >= static_cast<int>(cart.size())) {
        std::cout << "[Customer] Invalid cart index: " << cartIndex << "\n";
        return false;
    }
    
    Plant* plant = cart[cartIndex];
    if (plant == nullptr) {
        std::cout << "[Customer] No plant at cart index " << cartIndex << "\n";
        return false;
    }
    
    if (mediator == nullptr) {
        std::cout << "[Customer] No mediator available.\n";
        return false;
    }
    
    std::cout << "[Customer] " << getName() << " returning plant " 
              << plant->getID() << " to sales floor\n";
    
    // Check if plant is decorated and strip decorations
    Plant* plantToReturn = plant;
    if (Decorator::isDecorated(plant)) {
        std::cout << "[Customer] Plant is decorated. Stripping decorations before return...\n";
        plantToReturn = Decorator::stripDecorations(plant);
        
        if (plantToReturn == nullptr) {
            std::cout << "[Customer] Error stripping decorations.\n";
            return false;
        }
        
        // Update cart pointer to null since we've handled the decorated plant
        cart[cartIndex] = nullptr;
        std::cout << "[Customer] Decorations removed. Returning base plant.\n";
    }
    
    // Try to return via mediator
    bool success = mediator->returnPlantToDisplay(plantToReturn);
    
    if (success) {
        // Remove from cart (either nullptr or the plant if it wasn't decorated)
        cart.erase(cart.begin() + cartIndex);
        std::cout << "[Customer] Successfully returned plant to sales floor.\n";
    } else {
        std::cout << "[Customer] Failed to return plant to sales floor.\n";
        // If we stripped decorations but return failed, we have a problem
        // The base plant exists but decorators are deleted
        if (Decorator::isDecorated(plant)) {
            // Restore the plant pointer if return failed
            cart[cartIndex] = plantToReturn;
        }
    }
    
    return success;
}

void Customer::clearCart() {
    // Delete all plants in cart
    for (Plant* plant : cart) {
        delete plant;
    }
    cart.clear();
    std::cout << "[Customer] Cart cleared.\n";
}

std::vector<Plant*> Customer::getCart() const {
    return cart;
}

int Customer::getCartSize() const {
    return cart.size();
}

// ============ PLANT DECORATION (operates on cart) ============

Plant* Customer::getPlantFromCart(int index) const {
    if (index < 0 || index >= static_cast<int>(cart.size())) {
        std::cout << "[Customer] Invalid cart index: " << index << "\n";
        return nullptr;
    }
    return cart[index];
}

void Customer::decorateCartItemWithRibbon(int index) {
    if (index < 0 || index >= static_cast<int>(cart.size())) {
        std::cout << "[Customer] Invalid cart index: " << index << "\n";
        return;
    }
    
    Plant* originalPlant = cart[index];
    if (originalPlant == nullptr) {
        std::cout << "[Customer] No plant at index " << index << "\n";
        return;
    }
    
    Plant* decorated = new RibbonDecorator(originalPlant);
    cart[index] = decorated;
    
    std::cout << "[Customer] Added ribbon to plant at cart position " << index 
              << ". New price: R" << decorated->getPrice() << "\n";
}

void Customer::decorateCartItemWithGiftWrap(int index) {
    if (index < 0 || index >= static_cast<int>(cart.size())) {
        std::cout << "[Customer] Invalid cart index: " << index << "\n";
        return;
    }
    
    Plant* originalPlant = cart[index];
    if (originalPlant == nullptr) {
        std::cout << "[Customer] No plant at index " << index << "\n";
        return;
    }
    
    Plant* decorated = new GiftWrapDecorator(originalPlant);
    cart[index] = decorated;
    
    std::cout << "[Customer] Added gift wrap to plant at cart position " << index 
              << ". New price: R" << decorated->getPrice() << "\n";
}

void Customer::decorateCartItemWithPot(int index, std::string color) {
    if (index < 0 || index >= static_cast<int>(cart.size())) {
        std::cout << "[Customer] Invalid cart index: " << index << "\n";
        return;
    }
    
    Plant* originalPlant = cart[index];
    if (originalPlant == nullptr) {
        std::cout << "[Customer] No plant at index " << index << "\n";
        return;
    }
    
    Plant* decorated = new DecorativePotDecorator(originalPlant, color);
    cart[index] = decorated;
    
    std::cout << "[Customer] Added " << color << " pot to plant at cart position " << index 
              << ". New price: R" << decorated->getPrice() << "\n";
}

// ============ ORDER BUILDING (Composite Pattern) ============

void Customer::startNewOrder(const std::string& orderName) {
    if (currentOrder != nullptr) {
        std::cout << "[Customer] Warning: Deleting existing order to start new one.\n";
        delete currentOrder;
    }
    
    currentOrder = new ConcreteOrder(orderName);
    std::cout << "[Customer] Started new order: " << orderName << "\n";
}

void Customer::addCartItemToOrder(int index) {
    if (currentOrder == nullptr) {
        std::cout << "[Customer] No active order. Call startNewOrder() first.\n";
        return;
    }
    
    if (index < 0 || index >= static_cast<int>(cart.size())) {
        std::cout << "[Customer] Invalid cart index: " << index << "\n";
        return;
    }
    
    Plant* plant = cart[index];
    if (plant == nullptr) {
        std::cout << "[Customer] No plant at cart index " << index << "\n";
        return;
    }
    
    // Create Leaf with ownsPlant=false (cart still owns the plant)
    Leaf* leaf = new Leaf(plant, false);
    currentOrder->add(leaf);
    
    std::cout << "[Customer] Added plant from cart position " << index << " to order\n";
}

void Customer::addEntireCartToOrder() {
    if (currentOrder == nullptr) {
        std::cout << "[Customer] No active order. Call startNewOrder() first.\n";
        return;
    }
    
    if (cart.empty()) {
        std::cout << "[Customer] Cart is empty. Nothing to add to order.\n";
        return;
    }
    
    std::cout << "[Customer] Adding entire cart to order...\n";
    for (size_t i = 0; i < cart.size(); i++) {
        Plant* plant = cart[i];
        if (plant != nullptr) {
            Leaf* leaf = new Leaf(plant, false);
            currentOrder->add(leaf);
        }
    }
    std::cout << "[Customer] Added " << cart.size() << " plant(s) from cart to order.\n";
}

ConcreteOrder* Customer::getCurrentOrder() const {
    return currentOrder;
}

// ============ ORDER FINALIZATION (Prototype Pattern) ============

FinalOrder* Customer::createFinalOrder() {
    if (currentOrder == nullptr) {
        std::cout << "[Customer] No order to finalize.\n";
        return nullptr;
    }
    
    FinalOrder* finalOrder = new FinalOrder(getName());
    finalOrder->addOrder(currentOrder);
    
    std::cout << "[Customer] Created final order for " << getName() << "\n";
    std::cout << "[Customer] Total: R" << finalOrder->calculateTotalPrice() << "\n";
    
    // Reset current order (FinalOrder now owns it)
    currentOrder = nullptr;
    
    return finalOrder;
}

double Customer::calculateTotal() const {
    if (currentOrder == nullptr) {
        return 0.0;
    }
    return currentOrder->getPrice();
}

// ============ STAFF INTERACTION (Chain of Responsibility) ============

Request* Customer::createRequest(const std::string& message) {
    if (currentRequest != nullptr) {
        delete currentRequest;
    }
    
    currentRequest = new Request(message, this);
    currentRequest->parseRequest(message);
    std::cout << "[Customer] Created request: " << message << "\n";
    return currentRequest;
}

void Customer::submitRequestToStaff(StaffMembers* firstHandler) {
    if (firstHandler == nullptr) {
        std::cout << "[Customer] No staff handler available.\n";
        return;
    }
    
    if (currentRequest == nullptr) {
        std::cout << "[Customer] No request to submit. Call createRequest() first.\n";
        return;
    }
    
    std::cout << "[Customer] " << getName() << " submitting request to staff...\n";
    firstHandler->handleRequest(currentRequest);
}

void Customer::receiveResponse(const std::string& response) {
    std::cout << "[Customer] " << getName() << " received response: " << response << "\n";
    
    if (currentRequest != nullptr) {
        currentRequest->markHandled();
    }
}

Request* Customer::getCurrentRequest() const {
    return currentRequest;
}

// ============ BUDGET OPERATIONS ============

bool Customer::canAfford(double amount) const {
    return budget >= amount;
}

double Customer::getBudget() const {
    return budget;
}

void Customer::setBudget(double amount) {
    if (amount >= 0) {
        budget = amount;
        std::cout << "[Customer] Budget set to R" << budget << "\n";
    }
}

bool Customer::deductFromBudget(double amount) {
    if (amount < 0) {
        std::cout << "[Customer] Cannot deduct negative amount.\n";
        return false;
    }
    
    if (!canAfford(amount)) {
        std::cout << "[Customer] Insufficient funds. Budget: R" << budget 
                  << ", Required: R" << amount << "\n";
        return false;
    }
    
    budget -= amount;
    std::cout << "[Customer] Deducted R" << amount << " from budget. Remaining: R" 
              << budget << "\n";
    return true;
}

// ============ PRIVATE METHODS (friend access only) ============

void Customer::addToCart(Plant* plant) {
    if (plant == nullptr) {
        std::cout << "[Customer] Cannot add null plant to cart.\n";
        return;
    }
    
    auto it = std::find(cart.begin(), cart.end(), plant);
    if (it != cart.end()) {
        std::cout << "[Customer] Plant already in cart.\n";
        return;
    }
    
    cart.push_back(plant);
    std::cout << "[Customer] Added plant " << plant->getID() << " to cart.\n";
}

void Customer::removeFromCart(Plant* plant) {
    if (plant == nullptr) {
        return;
    }
    
    auto it = std::find(cart.begin(), cart.end(), plant);
    if (it != cart.end()) {
        cart.erase(it);
        std::cout << "[Customer] Removed plant " << plant->getID() << " from cart.\n";
    }
}