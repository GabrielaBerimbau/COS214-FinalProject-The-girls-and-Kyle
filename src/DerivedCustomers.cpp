#include "include/DerivedCustomers.h"
#include "include/PaymentProcessor.h"
#include "include/CashPayment.h"
#include "include/CreditCardPayment.h"
#include "include/FinalOrder.h"
#include "include/ConcreteOrder.h"
#include "include/Plant.h"
#include <iostream>

// ============ CorporateCustomer ============

CorporateCustomer::CorporateCustomer()
    : Customer(nullptr, "Corporate Client", "CORP-001", 1000.0) {
}

CorporateCustomer::~CorporateCustomer() {
}

void CorporateCustomer::checkOut() {
    std::cout << "\n=== Corporate Customer Checkout ===\n";
    
    if (cart.empty()) {
        std::cout << "Cart is empty! Nothing to purchase.\n";
        return;
    }
    
    std::cout << "Building order from cart...\n";
    startNewOrder("Corporate Order - " + getName());
    
    for (Plant* plant : cart) {
        if (plant != nullptr) {
            addPlantToOrder(plant);
        }
    }
    
    FinalOrder* finalOrder = createFinalOrder();
    if (finalOrder == nullptr) {
        std::cout << "Failed to create final order.\n";
        return;
    }
    
    double total = finalOrder->calculateTotalPrice();
    std::cout << "Order total: R" << total << "\n";
    std::cout << "Budget: R" << budget << "\n";
    
    if (!canAfford(total)) {
        std::cout << "Insufficient funds for purchase!\n";
        delete finalOrder;
        return;
    }
    
    if (getCartSize() > 10) {
        std::cout << "Large bulk order detected. Processing approval...\n";
    }
    
    if (mediator) {
        mediator->processPurchase();
    }
    
    PaymentProcessor* processor = new CreditCardPayment();
    processor->processTransaction(finalOrder);
    
    if (deductFromBudget(total)) {
        std::cout << "Corporate purchase successful!\n";
        clearCart();
    }
    
    delete processor;
    delete finalOrder;
}

// ============ RegularCustomer ============

RegularCustomer::RegularCustomer()
    : Customer(nullptr, "Regular Customer", "REG-001", 300.0) {
}

RegularCustomer::~RegularCustomer() {
}

void RegularCustomer::checkOut() {
    std::cout << "\n=== Regular Customer Checkout ===\n";
    
    if (cart.empty()) {
        std::cout << "Cart is empty! Nothing to purchase.\n";
        return;
    }
    
    std::cout << "Building order from cart...\n";
    startNewOrder("Order - " + getName());
    
    for (Plant* plant : cart) {
        if (plant != nullptr) {
            addPlantToOrder(plant);
        }
    }
    
    FinalOrder* finalOrder = createFinalOrder();
    if (finalOrder == nullptr) {
        std::cout << "Failed to create final order.\n";
        return;
    }
    
    double total = finalOrder->calculateTotalPrice();
    std::cout << "Order total: R" << total << "\n";
    std::cout << "Budget: R" << budget << "\n";
    
    if (!canAfford(total)) {
        std::cout << "Insufficient funds for purchase!\n";
        delete finalOrder;
        return;
    }
    
    if (mediator) {
        mediator->processPurchase();
    }
    
    PaymentProcessor* processor = new CreditCardPayment();
    processor->processTransaction(finalOrder);
    
    if (deductFromBudget(total)) {
        std::cout << "Purchase successful! Thank you for shopping with us.\n";
        clearCart();
    }
    
    delete processor;
    delete finalOrder;
}

// ============ WalkInCustomer ============

WalkInCustomer::WalkInCustomer()
    : Customer(nullptr, "Walk-In Customer", "WALK-001", 150.0) {
}

WalkInCustomer::~WalkInCustomer() {
}

void WalkInCustomer::checkOut() {
    std::cout << "\n=== Walk-In Customer Checkout ===\n";
    
    if (cart.empty()) {
        std::cout << "Cart is empty! Nothing to purchase.\n";
        return;
    }
    
    std::cout << "Building order from cart...\n";
    startNewOrder("Walk-In Order - " + getName());
    
    for (Plant* plant : cart) {
        if (plant != nullptr) {
            addPlantToOrder(plant);
        }
    }
    
    FinalOrder* finalOrder = createFinalOrder();
    if (finalOrder == nullptr) {
        std::cout << "Failed to create final order.\n";
        return;
    }
    
    double total = finalOrder->calculateTotalPrice();
    std::cout << "Order total: R" << total << "\n";
    std::cout << "Budget: R" << budget << "\n";
    
    if (!canAfford(total)) {
        std::cout << "Insufficient funds for purchase!\n";
        delete finalOrder;
        return;
    }
    
    if (mediator) {
        mediator->processPurchase();
    }
    
    PaymentProcessor* processor = new CashPayment();
    processor->processTransaction(finalOrder);
    
    if (deductFromBudget(total)) {
        std::cout << "Purchase complete! Have a great day.\n";
        clearCart();
    }
    
    delete processor;
    delete finalOrder;
}