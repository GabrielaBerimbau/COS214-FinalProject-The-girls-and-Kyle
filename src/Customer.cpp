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
#include <algorithm>
#include <iostream>

Customer::Customer(NurseryMediator* m, const std::string& name, 
                   const std::string& id, double initialBudget)
    : Person(m, name, id), budget(initialBudget), currentRequest(nullptr), currentOrder(nullptr) {
}

Customer::~Customer() {
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

// ============ Cart Management ============

void Customer::addToCart(Plant* plant) {
    if (plant == nullptr) {
        std::cout << "Cannot add null plant to cart.\n";
        return;
    }
    
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

// ============ Plant Decoration ============

Plant* Customer::decoratePlantWithRibbon(Plant* plant) {
    if (plant == nullptr) {
        std::cout << "Cannot decorate null plant.\n";
        return nullptr;
    }
    
    Plant* decorated = new RibbonDecorator(plant);
    std::cout << "Added ribbon decoration to plant.\n";
    return decorated;
}

Plant* Customer::decoratePlantWithGiftWrap(Plant* plant) {
    if (plant == nullptr) {
        std::cout << "Cannot decorate null plant.\n";
        return nullptr;
    }
    
    Plant* decorated = new GiftWrapDecorator(plant);
    std::cout << "Added gift wrap to plant.\n";
    return decorated;
}

Plant* Customer::decoratePlantWithPot(Plant* plant, std::string color) {
    if (plant == nullptr) {
        std::cout << "Cannot decorate null plant.\n";
        return nullptr;
    }
    
    Plant* decorated = new DecorativePotDecorator(plant, color);
    std::cout << "Added " << color << " decorative pot to plant.\n";
    return decorated;
}

// ============ Order Building (Composite Pattern) ============

void Customer::startNewOrder(const std::string& orderName) {
    if (currentOrder != nullptr) {
        delete currentOrder;
    }
    
    currentOrder = new ConcreteOrder(orderName);
    std::cout << "Started new order: " << orderName << "\n";
}

void Customer::addPlantToOrder(Plant* plant, int quantity) {
    if (currentOrder == nullptr) {
        std::cout << "No active order. Call startNewOrder() first.\n";
        return;
    }
    
    if (plant == nullptr) {
        std::cout << "Cannot add null plant to order.\n";
        return;
    }

    for (int i = 0; i < quantity; i++) {
        Leaf* leaf = new Leaf(plant, false);
        currentOrder->add(leaf);
    }
    
    std::cout << "Added " << quantity << " plant(s) to order\n";
}

ConcreteOrder* Customer::getCurrentOrder() const {
    return currentOrder;
}

// ============ Final Order Creation (Prototype Pattern) ============

FinalOrder* Customer::createFinalOrder() {
    if (currentOrder == nullptr) {
        std::cout << "No order to finalize.\n";
        return nullptr;
    }
    
    FinalOrder* finalOrder = new FinalOrder(getName());
    finalOrder->addOrder(currentOrder);
    
    std::cout << "Created final order for " << getName() << "\n";
    std::cout << "Total: R" << finalOrder->calculateTotalPrice() << "\n";
    
    currentOrder = nullptr;
    
    return finalOrder;
}

// ============ Budget Operations ============

double Customer::calculateTotal() const {
    if (currentOrder == nullptr) {
        return 0.0;
    }
    return currentOrder->getPrice();
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
        std::cout << "Insufficient funds. Budget: R" << budget 
                  << ", Required: R" << amount << "\n";
        return false;
    }
    
    budget -= amount;
    std::cout << "Deducted R" << amount << " from budget. Remaining: R" 
              << budget << "\n";
    return true;
}

// ============ Request/Staff Interaction ============

Request* Customer::createRequest(const std::string& message) {
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
}

void Customer::receiveResponse(const std::string& response) {
    std::cout << getName() << " received response: " << response << "\n";
    
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