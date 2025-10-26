#include "include/Aloe.h"

Aloe::Aloe(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety)
    : Succulent("Aloe", id, careStrategy, initialState) {
}

Aloe::~Aloe() {
}

std::string Aloe::toString() const {
    return "Aloe - " + Succulent::toString();
}