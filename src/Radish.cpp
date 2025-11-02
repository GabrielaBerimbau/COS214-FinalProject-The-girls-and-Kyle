#include "include/Radish.h"

Radish::Radish(const std::string& id, CareStrategy* careStrategy,
               PlantState* initialState, const std::string& variety,
               const std::string& color)
    : Vegetable("Radish", id, careStrategy, initialState) {
        (void)variety;
        (void)color;
}

Radish::~Radish() {
}

std::string Radish::toString() const {
    return "Radish - " + Vegetable::toString();
}