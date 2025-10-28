#include "include/Decorator.h"

Decorator::Decorator(Plant* p) 
    : Plant(p->getName(), p->getID(), nullptr, nullptr), plant(p) {
}

Decorator::~Decorator() {
    delete plant;
}

void Decorator::dailyUpdate() {
    if (plant != nullptr) {
        plant->dailyUpdate();
    }
}

std::string Decorator::toString() const {
    if (plant != nullptr) {
        return plant->toString();
    }
    return "Empty Decorator";
}