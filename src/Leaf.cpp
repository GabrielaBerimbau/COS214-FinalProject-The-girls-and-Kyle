#include "include/Leaf.h"
#include "include/Iterator.h"
#include "include/ConcreteIterator.h"

Leaf::Leaf(Plant *p, bool owns) : plant(p), ownsPlant(owns)
{
}

Leaf::~Leaf()
{
    if (ownsPlant && plant != nullptr) {
        delete plant;
    }
}

double Leaf::getPrice()
{
    // Return the price of this single physical plant
    if (plant) {
        return plant->getPrice();
    }
    return 0.0;
}

std::string Leaf::description()
{
    return plant->description();
}

void Leaf::add(Order *order)
{
    (void)order;
    throw std::runtime_error("Cannot add to a Leaf.");
}

void Leaf::remove(Order *order)
{
    (void)order;
    throw std::runtime_error("Cannot remove from a Leaf.");
}

Order* Leaf::clone() const {
    // Don't clone the plant, just copy the pointer
    // The cloned leaf should NOT own the plant to avoid double-delete
    return new Leaf(plant, false);
}

std::string Leaf::getName() const {
    return plant->getName();
}

Iterator* Leaf::createIterator() {
    return new ConcreteIterator(this);
}