#include "include/Carrot.h"

Carrot::Carrot(const std::string& id, CareStrategy* careStrategy,
               PlantState* initialState, const std::string& variety,
               const std::string& skin)
    : Vegetable("Carrot", id, careStrategy, initialState) {
        (void)variety;
        (void)skin;
}

Carrot::~Carrot() {
}

std::string Carrot::toString() const {
    return "Carrot - " + Vegetable::toString();
}