#ifndef FLOWERINGSTATE_H
#define FLOWERINGSTATE_H

#include "PlantState.h"

class Plant;

class FloweringState : public PlantState {
public:
    FloweringState();
    
    ~FloweringState();
    
    void handleChange(Plant* plant) override;

    std::string getStateName() override;
};

#endif