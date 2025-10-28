#include "include/Decorator.h"

Decorator::Decorator(Plant* p) 
    : Plant(*p), plant(p) {
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

double Decorator::getPrice() const {
    if (plant) {
        return plant->getPrice();
    }
    return 0.0;
}

std::string Decorator::description() const {
    if (plant) {
        return plant->description();
    }
    return "";
}