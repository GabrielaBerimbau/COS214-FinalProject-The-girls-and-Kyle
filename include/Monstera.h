#ifndef MONSTERA_H
#define MONSTERA_H

#include "OtherPlant.h"
#include <string>

class Monstera : public OtherPlant {
public:
    Monstera(const std::string& id, CareStrategy* careStrategy,
             PlantState* initialState, int initialLeaves = 3);
    
    ~Monstera();
    
    std::string toString() const override;
};

#endif





        