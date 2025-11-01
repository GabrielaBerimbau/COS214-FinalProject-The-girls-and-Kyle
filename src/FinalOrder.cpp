/**
 * @file FinalOrder.cpp
 * @author Gabi de Gouveia
 * @brief Implements the concrete Prototype class for customer orders.
 * 
 * This file defines all logic for creating, cloning, summarizing, and printing
 * final customer orders. It demonstrates collaboration between the **Prototype**
 * and **Iterator** design patterns.
 */

#include "include/FinalOrder.h"
#include "include/Iterator.h"
#include "include/ConcreteIterator.h"
#include "include/ConcreteOrder.h"
#include <iostream>
#include <sstream>
#include <iomanip>

FinalOrder::FinalOrder(const std::string& name)
    : customerName(name), totalPrice(0.0) {}

FinalOrder::FinalOrder(const FinalOrder& other)
    : customerName(other.customerName), totalPrice(other.totalPrice) {
    for (auto* o : other.orderList) {
        if (o) {
            orderList.push_back(o->clone());
        }
    }
}

FinalOrder::~FinalOrder() {
    for (auto* o : orderList) {
        delete o;
    }
    orderList.clear();
}

FinalOrder* FinalOrder::clone() const {
    std::cout << "[Prototype] Cloning FinalOrder for " << customerName << " using Iterator...\n";
    FinalOrder* copy = new FinalOrder(customerName);
    
    for (auto* order : orderList) {
        if (order) {
            copy->addOrder(order->clone());
        }
    }
    
    return copy;
}

void FinalOrder::addOrder(Order* order) {
    if (order) {
        orderList.push_back(order);
        totalPrice += order->getPrice();
    }
}

double FinalOrder::calculateTotalPrice() const {
    double total = 0.0;
    
    for (auto* order : orderList) {
        if (order) {
            Iterator* it = order->createIterator();
            it->first();
            while (!it->isDone()) {
                Order* item = it->currentItem();
                if (item) {
                    total += item->getPrice();
                }
                it->next();
            }
            delete it;
        }
    }
    
    return total;
}

std::string FinalOrder::getSummary() const {
    std::string summary = "Order Summary for " + customerName + ":\n";
    
    for (auto* order : orderList) {
        if (order) {
            summary += "- " + order->getName() + ":\n";
            
            Iterator* it = order->createIterator();
            it->first();
            while (!it->isDone()) {
                Order* item = it->currentItem();
                if (item) {
                    summary += "  * " + item->getName() + ": R" + std::to_string(item->getPrice()) + "\n";
                }
                it->next();
            }
            delete it;
        }
    }
    
    summary += "Total: R" + std::to_string(calculateTotalPrice()) + "\n";
    return summary;
}

void FinalOrder::printInvoice() const {
    std::cout << "---------------------------------------\n";
    std::cout << "Invoice for: " << customerName << "\n";

    for (auto* order : orderList) {
        if (order) {
            std::cout << order->getName() << ":\n";

            Iterator* it = order->createIterator();
            it->first();
            while (!it->isDone()) {
                Order* item = it->currentItem();
                if (item) {
                    std::cout << "  - " << item->getName() << " : R" << item->getPrice() << "\n";
                }
                it->next();
            }
            delete it;
        }
    }

    std::cout << "Total: R" << calculateTotalPrice() << "\n";
    std::cout << "---------------------------------------\n";
}

void FinalOrder::printOrderStructure() const {
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Final Order for: " << customerName;
    for(size_t i = customerName.length(); i < 38; i++) std::cout << " ";
    std::cout << "║\n";
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";

    if (orderList.empty()) {
        std::cout << "║  (No orders)";
        for(int i = 0; i < 46; i++) std::cout << " ";
        std::cout << "║\n";
    } else {
        for (auto* order : orderList) {
            if (order) {
                order->printStructure(0, "");
            }
        }
    }

    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  TOTAL: R" << calculateTotalPrice();
    std::string totalStr = std::to_string(calculateTotalPrice());
    for(size_t i = totalStr.length(); i < 48; i++) std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
}

std::string FinalOrder::getFormattedReceipt() const {
    std::string receipt = "";

    if (orderList.empty()) {
        receipt += "(No orders)\n";
        return receipt;
    }

    for (size_t i = 0; i < orderList.size(); i++) {
        Order* order = orderList[i];
        if (order) {
            bool isLast = (i == orderList.size() - 1);
            receipt += formatOrderRecursive(order, 0, isLast);
        }
    }

    return receipt;
}

std::string FinalOrder::formatOrderRecursive(Order* order, int indent, bool isLast) const {
    if (!order) return "";

    std::string result = "";
    std::string indentStr = "";

    // Build indentation string
    for (int i = 0; i < indent; i++) {
        indentStr += "  ";
    }

    // Try to cast to ConcreteOrder to check if it's a composite
    ConcreteOrder* composite = dynamic_cast<ConcreteOrder*>(order);

    if (composite) {
        // This is a composite order (suborder)
        std::vector<Order*> children = composite->getChildren();

        // Add the suborder name
        result += indentStr + "[" + composite->getName() + "]\n";

        // Recursively format children
        for (size_t i = 0; i < children.size(); i++) {
            Order* child = children[i];
            if (child) {
                bool childIsLast = (i == children.size() - 1);
                result += formatOrderRecursive(child, indent + 1, childIsLast);
            }
        }
    } else {
        // This is a leaf item (individual plant)
        std::ostringstream priceStream;
        priceStream << std::fixed << std::setprecision(2) << order->getPrice();

        result += indentStr + "- " + order->getName() + ": R" + priceStream.str() + "\n";
    }

    return result;
}