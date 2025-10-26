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
    
    double total = calculateTotal();
    std::cout << "Cart total: $" << total << "\n";
    std::cout << "Budget: $" << budget << "\n";
    
    if (!canAfford(total)) {
        std::cout << "Insufficient funds for purchase!\n";
        return;
    }
    
    // Corporate customers may need approval for large orders
    if (getCartSize() > 10) {
        std::cout << "Large bulk order detected. Processing approval...\n";
    }
    
    // Create FinalOrder from cart
    FinalOrder* finalOrder = new FinalOrder(getName());
    
    // Add each plant from cart as a ConcreteOrder
    for (Plant* plant : cart) {
        if (plant != nullptr) {
            std::vector<std::string> items;
            items.push_back(plant->getName());
            ConcreteOrder* order = new ConcreteOrder(
                plant->getName(), 
                items, 
                plant->getPrice()
            );
            finalOrder->addOrder(order);
        }
    }
    
    // Notify mediator of purchase
    if (mediator) {
        mediator->processPurchase();
    }
    
    // Use credit card payment for corporate
    PaymentProcessor* processor = new CreditCardPayment();
    processor->processTransaction(finalOrder);
    
    // Deduct from budget
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
    
    double total = calculateTotal();
    std::cout << "Cart total: $" << total << "\n";
    std::cout << "Budget: $" << budget << "\n";
    
    if (!canAfford(total)) {
        std::cout << "Insufficient funds for purchase!\n";
        return;
    }
    
    // Create FinalOrder from cart
    FinalOrder* finalOrder = new FinalOrder(getName());
    
    // Add each plant from cart as a ConcreteOrder
    for (Plant* plant : cart) {
        if (plant != nullptr) {
            std::vector<std::string> items;
            items.push_back(plant->getName());
            ConcreteOrder* order = new ConcreteOrder(
                plant->getName(), 
                items, 
                plant->getPrice()
            );
            finalOrder->addOrder(order);
        }
    }
    
    // Notify mediator of purchase
    if (mediator) {
        mediator->processPurchase();
    }
    
    // Regular customers can choose payment method
    // For now, default to credit card
    PaymentProcessor* processor = new CreditCardPayment();
    processor->processTransaction(finalOrder);
    
    // Deduct from budget
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
    
    double total = calculateTotal();
    std::cout << "Cart total: $" << total << "\n";
    std::cout << "Budget: $" << budget << "\n";
    
    if (!canAfford(total)) {
        std::cout << "Insufficient funds for purchase!\n";
        return;
    }
    
    // Create FinalOrder from cart
    FinalOrder* finalOrder = new FinalOrder(getName());
    
    // Add each plant from cart as a ConcreteOrder
    for (Plant* plant : cart) {
        if (plant != nullptr) {
            std::vector<std::string> items;
            items.push_back(plant->getName());
            ConcreteOrder* order = new ConcreteOrder(
                plant->getName(), 
                items, 
                plant->getPrice()
            );
            finalOrder->addOrder(order);
        }
    }
    
    // Notify mediator of purchase
    if (mediator) {
        mediator->processPurchase();
    }
    
    // Walk-in customers typically pay cash
    PaymentProcessor* processor = new CashPayment();
    processor->processTransaction(finalOrder);
    
    // Deduct from budget
    if (deductFromBudget(total)) {
        std::cout << "Purchase complete! Have a great day.\n";
        clearCart();
    }
    
    delete processor;
    delete finalOrder;
}