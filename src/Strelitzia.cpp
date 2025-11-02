#include "include/Strelitzia.h"

Strelitzia::Strelitzia(const std::string& id, CareStrategy* careStrategy,
                       PlantState* initialState)
    : Flower("Strelitzia", id, careStrategy, initialState) {
}

Strelitzia::~Strelitzia() {
}

std::string Strelitzia::toString() const {
    return "Strelitzia (Bird of Paradise) - " + Flower::toString();
}