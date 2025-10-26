/**
 * @file FinalOrder.cpp
 * @brief Implements the FinalOrder Prototype class.
 */

#include "include/FinalOrder.h"
#include <iostream>

FinalOrder::FinalOrder(const std::string& name)
    : customerName(name), totalPrice(0.0) {}

FinalOrder::FinalOrder(const FinalOrder& other)
    : customerName(other.customerName), totalPrice(other.totalPrice) {
    for (auto* o : other.orderList) {
        orderList.push_back(new Order(*o));  // Deep copy
    }
}

FinalOrder::~FinalOrder() {
    for (auto* o : orderList) {
        delete o;
    }
    orderList.clear();
}

FinalOrder* FinalOrder::clone() const {
    std::cout << "[Prototype] Cloning FinalOrder for " << customerName << "...\n";
    return new FinalOrder(*this);
}

void FinalOrder::addOrder(Order* order) {
    if (order) {
        orderList.push_back(order);
        totalPrice += order->getPrice();
    }
}

double FinalOrder::calculateTotalPrice() const {
    double total = 0.0;
    for (auto* o : orderList) {
        total += o->getPrice();
    }
    return total;
}

std::string FinalOrder::getSummary() const {
    std::string summary = "Order Summary for " + customerName + ":\n";
    for (auto* o : orderList) {
        summary += "- " + o->getName() + ": R" + std::to_string(o->getPrice()) + "\n";
    }
    summary += "Total: R" + std::to_string(calculateTotalPrice()) + "\n";
    return summary;
}

void FinalOrder::printInvoice() const {
    std::cout << "---------------------------------------\n";
    std::cout << "Invoice for: " << customerName << "\n";
    for (auto* o : orderList) {
        std::cout << "- " << o->getName() << " : R" << o->getPrice() << "\n";
    }
    std::cout << "Total: R" << calculateTotalPrice() << "\n";
    std::cout << "---------------------------------------\n";
}
