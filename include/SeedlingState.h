#ifndef SEEDLINGSTATE_H
#define SEEDLINGSTATE_H

#include "PlantState.h"

class Plant;

class SeedlingState : public PlantState {
public:
    SeedlingState();
    
    ~SeedlingState();
    
    void handleChange(Plant* plant) override;
};

#endif