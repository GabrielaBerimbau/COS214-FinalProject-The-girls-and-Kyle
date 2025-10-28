#include "include/Potato.h"

Potato::Potato(const std::string& id, CareStrategy* careStrategy,
               PlantState* initialState, const std::string& variety,
               const std::string& skin)
    : Vegetable("Potato", id, careStrategy, initialState) {
}

Potato::~Potato() {
}

std::string Potato::toString() const {
    return "Potato - " + Vegetable::toString();
}