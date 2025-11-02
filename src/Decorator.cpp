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

Plant* Decorator::getWrappedPlant() const {
    return plant;
}

Plant* Decorator::getBasePlant() {
    Plant* current = this->plant;
    
    while (current != nullptr) {
        Decorator* decorator = dynamic_cast<Decorator*>(current);
        if (decorator == nullptr) {
            // Found the base plant (not a decorator)
            return current;
        }
        // Move to next wrapped plant
        current = decorator->plant;
    }
    
    return nullptr;
}

Plant* Decorator::stripDecorations(Plant* plant) {
    if (plant == nullptr) {
        return nullptr;
    }
    
    // Check if this is a decorator
    Decorator* decorator = dynamic_cast<Decorator*>(plant);
    
    if (decorator == nullptr) {
        // Already a base plant, no decorations to strip
        return plant;
    }
    
    // This is a decorator, get the base plant
    Plant* basePlant = decorator->getBasePlant();
    
    if (basePlant == nullptr) {
        return nullptr;
    }
    
    // Remove the base plant from the decorator chain
    // We need to traverse and set the innermost decorator's plant to nullptr
    Decorator* current = decorator;
    Decorator* previous = nullptr;
    
    while (current != nullptr) {
        Decorator* nextDecorator = dynamic_cast<Decorator*>(current->plant);
        if (nextDecorator == nullptr) {
            // current->plant is the base plant
            current->plant = nullptr;  // Detach it so it won't be deleted
            break;
        }
        previous = current;
        current = nextDecorator;
    }
    
    // Now delete the decorator chain (base plant is detached)
    delete decorator;
    
    return basePlant;
}

bool Decorator::isDecorated(Plant* plant) {
    if (plant == nullptr) {
        return false;
    }
    
    Decorator* decorator = dynamic_cast<Decorator*>(plant);
    return (decorator != nullptr);
}