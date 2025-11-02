#include "include/VenusFlyTrap.h"

VenusFlyTrap::VenusFlyTrap(const std::string& id, CareStrategy* careStrategy,
                           PlantState* initialState, int initialTraps)
    : OtherPlant("Venus Fly Trap", id, careStrategy, initialState) {
        (void)initialTraps;
}

VenusFlyTrap::~VenusFlyTrap() {
}

std::string VenusFlyTrap::toString() const {
    return "Venus Fly Trap - " + OtherPlant::toString();
}