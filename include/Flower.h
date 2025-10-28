#ifndef FLOWER_H
#define FLOWER_H

#include "Plant.h"

class Flower : public Plant {
public:
    Flower(const std::string& name, const std::string& id,
           CareStrategy* careStrategy, PlantState* initialState);
    
    virtual ~Flower();
    
    void dailyUpdate() override;
    
    std::string toString() const override;
};

#endif