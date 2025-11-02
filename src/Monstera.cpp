
#include "include/Monstera.h"

Monstera::Monstera(const std::string& id, CareStrategy* careStrategy,
                   PlantState* initialState, int initialLeaves)
    : OtherPlant("Monstera", id, careStrategy, initialState) {
        (void)initialLeaves;
}

Monstera::~Monstera() {
}

std::string Monstera::toString() const {
    return "Monstera - " + OtherPlant::toString();
}
