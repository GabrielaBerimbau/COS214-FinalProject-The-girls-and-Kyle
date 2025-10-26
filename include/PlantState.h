#ifndef PLANTSTATE_H
#define PLANTSTATE_H

#include <string>

class Plant;

class PlantState {
public:
    virtual ~PlantState() {}
    
    virtual void handleChange(Plant* plant) = 0;

    virtual std::string getStateName() = 0;
    
protected:
    PlantState() {}
};

#endif