#include "include/Vegetable.h"

Vegetable::Vegetable(const std::string& name, const std::string& id,
                     CareStrategy* careStrategy, PlantState* initialState)
    : Plant(name, id, careStrategy, initialState) {
    setPrice(12.0);
}

Vegetable::~Vegetable() {
}

void Vegetable::dailyUpdate() {
    incrementAge();
    
    // Vegetables are heavy feeders and drinkers
    setWaterLevel(getWaterLevel() - 12);
    
    // Vegetables consume nutrients quickly
    setNutrientLevel(getNutrientLevel() - 10);
    
    updateHealth();
    notify();
    
    if (getState() != nullptr) {
        getState()->handleChange(this);
    }
}

std::string Vegetable::toString() const {
    return "Vegetable - " + Plant::toString();
}