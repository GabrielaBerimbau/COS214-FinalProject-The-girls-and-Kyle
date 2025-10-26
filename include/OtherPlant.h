#ifndef OTHERPLANT_H
#define OTHERPLANT_H

#include "Plant.h"

class OtherPlant : public Plant {
public:
    OtherPlant(const std::string& name, const std::string& id,
               CareStrategy* careStrategy, PlantState* initialState);
    
    virtual ~OtherPlant();
    
    std::string toString() const override;
};

#endif