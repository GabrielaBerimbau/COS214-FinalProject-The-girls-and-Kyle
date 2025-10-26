#include "include/Daisy.h"

Daisy::Daisy(const std::string& id, CareStrategy* careStrategy,
             PlantState* initialState, const std::string& petalColor,
             const std::string& varietyType)
    : Flower("Daisy", id, careStrategy, initialState) {
}

Daisy::~Daisy() {
}

std::string Daisy::toString() const {
    return "Daisy - " + Flower::toString();
}