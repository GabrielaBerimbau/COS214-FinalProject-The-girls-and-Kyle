#ifndef DEAD_STATE_H
#define DEAD_STATE_H

#include "PlantState.h"

class Plant;

class DeadState : public PlantState {
public:
    DeadState();
    
    ~DeadState();
    
    void handleChange(Plant* plant) override;

    std::string getStateName() override;
};

#endif