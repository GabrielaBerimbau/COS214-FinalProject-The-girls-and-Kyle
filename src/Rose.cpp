#include "include/Rose.h"

Rose::Rose(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& roseColor,
           const std::string& type)
    : Flower("Rose", id, careStrategy, initialState) {
        (void)roseColor;
        (void)type;
}

Rose::~Rose() {
}

std::string Rose::toString() const {
    return "Rose - " + Flower::toString();
}