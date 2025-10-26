#include "include/Succulent.h"

Succulent::Succulent(const std::string& name, const std::string& id,
                     CareStrategy* careStrategy, PlantState* initialState)
    : Plant(name, id, careStrategy, initialState) {
    setPrice(15.0);
}

Succulent::~Succulent() {
}

void Succulent::dailyUpdate() {
    incrementAge();
    
    // Succulents lose water SLOWER than other plants
    setWaterLevel(getWaterLevel() - 5);
    
    // Nutrients decay normally
    setNutrientLevel(getNutrientLevel() - 5);
    
    updateHealth();
    notify();
    
    if (getState() != nullptr) {
        getState()->handleChange(this);
    }
}

std::string Succulent::toString() const {
    return "Succulent - " + Plant::toString();
}