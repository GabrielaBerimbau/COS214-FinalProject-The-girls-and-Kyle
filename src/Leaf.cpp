#include "include/Leaf.h"
#include "include/Iterator.h"
#include "include/ConcreteIterator.h"

Leaf::Leaf(Plant *p, int q, bool owns) : plant(p), quantity(q), ownsPlant(owns)
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
    if (plant) {
        return plant->getPrice() * quantity;
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
    return new Leaf(plant, quantity, false);
}

std::string Leaf::getName() const {
    return plant->getName();
}

Iterator* Leaf::createIterator() {
    return new ConcreteIterator(this);
}