#ifndef DORMANTSTATE_H
#define DORMANTSTATE_H

#include "PlantState.h"

class Plant;

class DormantState : public PlantState {
public:
    DormantState();
    
    ~DormantState();
    
    void handleChange(Plant* plant) override;
};

#endif