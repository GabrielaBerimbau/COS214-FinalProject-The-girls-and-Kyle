#include "include/Flower.h"

Flower::Flower(const std::string& name, const std::string& id,
               CareStrategy* careStrategy, PlantState* initialState)
    : Plant(name, id, careStrategy, initialState) {
    setPrice(25.0);
}

Flower::~Flower() {
}

void Flower::dailyUpdate() {
    incrementAge();
    
    // Flowers lose water FASTER (blooming takes energy)
    setWaterLevel(getWaterLevel() - 15);
    
    // Flowers need more nutrients
    setNutrientLevel(getNutrientLevel() - 8);
    
    updateHealth();
    notify();
    
    if (getState() != nullptr) {
        getState()->handleChange(this);
    }
}

std::string Flower::toString() const {
    return "Flower - " + Plant::toString();
}