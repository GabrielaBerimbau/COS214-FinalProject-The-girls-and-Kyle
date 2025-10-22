#ifndef PLANTSTATE_H
#define PLANTSTATE_H

class Plant;

class PlantState {
public:
    virtual ~PlantState() {}
    
    virtual void handleChange(Plant* plant) = 0;
    
protected:
    PlantState() {}
};

#endif