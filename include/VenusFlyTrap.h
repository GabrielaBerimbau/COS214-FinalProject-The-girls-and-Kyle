#ifndef VENUSFLYTRAP_H
#define VENUSFLYTRAP_H

#include "OtherPlant.h"

class VenusFlyTrap : public OtherPlant {
public:
    VenusFlyTrap(const std::string& id, CareStrategy* careStrategy,
                 PlantState* initialState, int initialTraps = 5);

    ~VenusFlyTrap();

    std::string toString() const override;
};

#endif