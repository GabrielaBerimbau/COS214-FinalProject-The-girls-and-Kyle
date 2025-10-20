#ifndef MATURESTATE_H
#define MATURESTATE_H

#include "PlantState.h"

class Plant;

class MatureState : public PlantState {
public:
    MatureState();
    
    ~MatureState();
    
    void handleChange(Plant* plant) override;
};

#endif