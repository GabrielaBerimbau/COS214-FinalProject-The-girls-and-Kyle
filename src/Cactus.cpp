#include "include/Cactus.h"

Cactus::Cactus(const std::string& id, CareStrategy* careStrategy,
               PlantState* initialState, const std::string& shape,
               const std::string& variety)
    : Succulent("Cactus", id, careStrategy, initialState) {
}

Cactus::~Cactus() {
}

std::string Cactus::toString() const {
    return "Cactus - " + Succulent::toString();
}