#ifndef GROWINGSTATE_H
#define GROWINGSTATE_H

#include "PlantState.h"

class Plant;

class GrowingState : public PlantState {
public:
    GrowingState();
    
    ~GrowingState();
    
    void handleChange(Plant* plant) override;

    std::string getStateName() override;
};

#endif