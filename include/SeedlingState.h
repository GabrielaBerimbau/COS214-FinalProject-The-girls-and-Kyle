#ifndef SEEDLINGSTATE_H
#define SEEDLINGSTATE_H

#include "PlantState.h"
#include <string>

class Plant;

class SeedlingState : public PlantState {
public:
    SeedlingState();
    
    ~SeedlingState();
    
    void handleChange(Plant* plant) override;

    std::string getStateName() override;
};

#endif