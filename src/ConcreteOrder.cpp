#include "include/ConcreteOrder.h"
#include <iostream>

ConcreteOrder::ConcreteOrder(std::string name, std::vector<std::string> items, double price)
    : customerName(name), plantList(items), totalPrice(price) {
}

ConcreteOrder::ConcreteOrder(const ConcreteOrder& other)
    : customerName(other.customerName), 
      plantList(other.plantList), 
      totalPrice(other.totalPrice) {
}

Order* ConcreteOrder::clone() const {
    return new ConcreteOrder(*this);
}

void ConcreteOrder::display() const {
    std::cout << "Order for: " << customerName << "\n";
    std::cout << "Plants:\n";
    for (const auto& plant : plantList) {
        std::cout << "  - " << plant << "\n";
    }
    std::cout << "Total Price: R" << totalPrice << "\n";
}

std::string ConcreteOrder::getName() const {
    return customerName;
}

double ConcreteOrder::getPrice() const {
    return totalPrice;
}

void ConcreteOrder::setCustomerName(const std::string& name) {
    customerName = name;
}

void ConcreteOrder::addPlant(const std::string& plant) {
    plantList.push_back(plant);
}

double ConcreteOrder::getTotalPrice() const {
    return totalPrice;
}