#include "include/ConcreteOrder.h"
#include "include/Iterator.h"
#include "include/ConcreteIterator.h"
#include "include/Leaf.h"

ConcreteOrder::ConcreteOrder(std::string orderN) : orderName(orderN) 
{
}

ConcreteOrder::~ConcreteOrder() 
{
    // Delete all child orders
    for (Order* child : plantList) {
        delete child;
    }
    plantList.clear();
}

double ConcreteOrder::getPrice() const
{
    double total = 0.0;

    for (Order* plant : plantList) {
        if (plant) {
            total += plant->getPrice();
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

Order* ConcreteOrder::clone() const {
    ConcreteOrder* copy = new ConcreteOrder(orderName);
    for (auto* child : plantList) {
        if (child) {
            copy->add(child->clone());
        }
    }
    return copy;
}

std::string ConcreteOrder::getName() const {
    return orderName;
}

Iterator* ConcreteOrder::createIterator() {
    return new ConcreteIterator(this);
}

std::vector<Order*> ConcreteOrder::getChildren() const {
    return plantList;
}

void ConcreteOrder::printStructure(int indent, const std::string& prefix) const {
    std::string indentStr(indent * 2, ' ');
    std::cout << indentStr << prefix << "├─ [" << orderName << "] - R" << getPrice() << "\n";

    for (size_t i = 0; i < plantList.size(); ++i) {
        if (plantList[i]) {
            bool isLast = (i == plantList.size() - 1);
            std::string childPrefix = isLast ? "└─ " : "├─ ";

            ConcreteOrder* composite = dynamic_cast<ConcreteOrder*>(plantList[i]);
            Leaf* leaf = dynamic_cast<Leaf*>(plantList[i]);

            if (composite) {
                composite->printStructure(indent + 1, childPrefix);
            } else if (leaf) {
                leaf->printStructure(indent + 1, childPrefix);
            }
        }
    }
}