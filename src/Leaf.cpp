#include "include/Leaf.h"

Leaf::Leaf(Plant *p, int q) : plant(p), quantity(q)
{
}

Leaf::~Leaf()
{
}

double Leaf::getPrice()
{
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

// Order *Leaf::getChild(int index)
// {
//     return nullptr;
// }

