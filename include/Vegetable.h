#ifndef VEGETABLE_H
#define VEGETABLE_H

#include "Plant.h"

class Vegetable : public Plant {
public:
    Vegetable(const std::string& name, const std::string& id,
              CareStrategy* careStrategy, PlantState* initialState);
    
    virtual ~Vegetable();
    
    void dailyUpdate() override;
    
    std::string toString() const override;
};

#endif