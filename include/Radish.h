#ifndef RADISH_H
#define RADISH_H

#include "Vegetable.h"

class Radish : public Vegetable {
public:
    Radish(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety = "Cherry Belle",
           const std::string& color = "red");
    
    ~Radish();
    
    std::string toString() const override;
};

#endif