#include "include/OtherPlant.h"

OtherPlant::OtherPlant(const std::string& name, const std::string& id,
                       CareStrategy* careStrategy, PlantState* initialState)
    : Plant(name, id, careStrategy, initialState) {
    setPrice(20.0);
}

OtherPlant::~OtherPlant() {
}

std::string OtherPlant::toString() const {
    return "Other Plant - " + Plant::toString();
}