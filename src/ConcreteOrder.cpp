#include "include/ConcreteOrder.h"

ConcreteOrder::ConcreteOrder(std::string orderN) : orderName(orderN) 
{
}

ConcreteOrder::~ConcreteOrder() 
{
    // Memory management
}

double ConcreteOrder::getPrice()
{
    double total = 0.0;
    
    for (Order* plant : plantList) {
        if (plant) {
            total += plant->getPrice();  //Recursive call
        }
    }
    
    return total;
}

std::string ConcreteOrder::description() 
{
    std::ostringstream output;
    output << orderName;
    
    if (!plantList.empty()) {
        output << " [\n";
        for (size_t i = 0; i < plantList.size(); ++i) {
            if (plantList[i]) {
                output << "  " << plantList[i]->description();
                if (i < plantList.size() - 1) {
                    output << ",\n";
                }
            }
        }
        output << "\n]";
    }
    
    return output.str();
}

void ConcreteOrder::add(Order *order)
{
    if (order == nullptr) {
        return;
    }

    plantList.push_back(order);
}

void ConcreteOrder::remove(Order *order)
{
    if (order == nullptr) {
        return;
    }

    auto it = std::find(plantList.begin(), plantList.end(), order);
    if (it != plantList.end()) {
        plantList.erase(it);
    }
}