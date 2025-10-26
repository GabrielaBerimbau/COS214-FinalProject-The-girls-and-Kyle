#ifndef SUCCULENT_H
#define SUCCULENT_H

#include "Plant.h"


class Succulent : public Plant {
public:
    Succulent(const std::string& name, const std::string& id,
              CareStrategy* careStrategy, PlantState* initialState);
    
    virtual ~Succulent();
    
    void dailyUpdate() override;
    
    std::string toString() const override;
};

#endif