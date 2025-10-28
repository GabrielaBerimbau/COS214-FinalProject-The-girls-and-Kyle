#ifndef STRELITZIA_H
#define STRELITZIA_H

#include "Flower.h"

class Strelitzia : public Flower {
public:
    Strelitzia(const std::string& id, CareStrategy* careStrategy,
               PlantState* initialState);
    
    ~Strelitzia();
    
    std::string toString() const override;
};

#endif