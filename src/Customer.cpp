#include "include/Customer.h"
#include "include/Plant.h"
#include "include/Request.h"
#include "include/NurseryMediator.h"
#include <algorithm>
#include <iostream>

Customer::Customer(NurseryMediator* m, const std::string& name, 
                   const std::string& id, double initialBudget)
    : Person(m, name, id), budget(initialBudget), currentRequest(nullptr) {
}

Customer::~Customer() {
    // Don't delete plants in cart - we don't own them
    cart.clear();
    
    // Clean up current request if exists
    if (currentRequest != nullptr) {
        delete currentRequest;
        currentRequest = nullptr;
    }
}

// ============ Cart Management ============

void Customer::addToCart(Plant* plant) {
    if (plant == nullptr) {
        std::cout << "Cannot add null plant to cart.\n";
        return;
    }
    
    // Check if plant already in cart
    auto it = std::find(cart.begin(), cart.end(), plant);
    if (it != cart.end()) {
        std::cout << "Plant already in cart.\n";
        return;
    }
    
    cart.push_back(plant);
    std::cout << "Added plant " << plant->getID() << " to cart.\n";
}

void Customer::removeFromCart(Plant* plant) {
    if (plant == nullptr) {
        return;
    }
    
    auto it = std::find(cart.begin(), cart.end(), plant);
    if (it != cart.end()) {
        cart.erase(it);
        std::cout << "Removed plant " << plant->getID() << " from cart.\n";
    }
}

void Customer::clearCart() {
    cart.clear();
    std::cout << "Cart cleared.\n";
}

std::vector<Plant*> Customer::getCart() const {
    return cart;
}

int Customer::getCartSize() const {
    return cart.size();
}

// ============ Budget Operations ============

double Customer::calculateTotal() const {
    double total = 0.0;
    for (const Plant* plant : cart) {
        if (plant != nullptr) {
            total += plant->getPrice();
        }
    }
    return total;
}

bool Customer::canAfford(double amount) const {
    return budget >= amount;
}

double Customer::getBudget() const {
    return budget;
}

void Customer::setBudget(double amount) {
    if (amount >= 0) {
        budget = amount;
    }
}

bool Customer::deductFromBudget(double amount) {
    if (amount < 0) {
        std::cout << "Cannot deduct negative amount.\n";
        return false;
    }
    
    if (!canAfford(amount)) {
        std::cout << "Insufficient funds. Budget: $" << budget 
                  << ", Required: $" << amount << "\n";
        return false;
    }
    
    budget -= amount;
    std::cout << "Deducted $" << amount << " from budget. Remaining: $" 
              << budget << "\n";
    return true;
}

// ============ Request/Staff Interaction ============

Request* Customer::createRequest(const std::string& message) {
    // Clean up old request if exists
    if (currentRequest != nullptr) {
        delete currentRequest;
    }
    
    currentRequest = new Request(message);
    currentRequest->parseRequest(message);
    std::cout << "Created request: " << message << "\n";
    return currentRequest;
}

Request* Customer::getCurrentRequest() const {
    return currentRequest;
}

void Customer::submitRequest() {
    if (mediator == nullptr) {
        std::cout << "No mediator available to submit request.\n";
        return;
    }
    
    if (currentRequest == nullptr) {
        std::cout << "No request to submit.\n";
        return;
    }
    
    std::cout << "Submitting request through mediator...\n";
    mediator->notify(this);
    
    // The mediator will coordinate with appropriate staff
    // based on the Request's level
}

void Customer::receiveResponse(const std::string& response) {
    std::cout << getName() << " received response: " << response << "\n";
    
    // Mark current request as handled
    if (currentRequest != nullptr) {
        currentRequest->markHandled();
    }
}

// ============ Plant Interaction (via Mediator) ============

void Customer::requestPlantByName(const std::string& plantName) {
    if (mediator == nullptr) {
        std::cout << "No mediator available.\n";
        return;
    }
    
    std::cout << getName() << " requesting plant: " << plantName << "\n";
    
    Plant* plant = mediator->requestPlantFromStaff(plantName);
    
    if (plant != nullptr) {
        std::cout << "Plant found! Adding to cart.\n";
        addToCart(plant);
    } else {
        std::string msg = mediator->plantUnavailable();
        std::cout << msg << "\n";
    }
}

void Customer::browseSalesFloor() {
    if (mediator == nullptr) {
        std::cout << "No mediator available.\n";
        return;
    }
    
    std::cout << getName() << " browsing sales floor...\n";
    mediator->notify(this);
    
    // Mediator coordinates with SalesFloor to show available plants
    // This might trigger GUI updates in the game context
}

// ============ Legacy Methods ============

void Customer::giveCustomerPlant(Plant* plant) {
    if (plant != nullptr) {
        addToCart(plant);
    }
}

std::string Customer::plantNotInStock() {
    if (mediator != nullptr) {
        return mediator->plantUnavailable();
    }
    return "Plant is not available.";
}