#ifndef ALOE_H
#define ALOE_H

#include "Succulent.h"

class Aloe : public Succulent {
public:
    Aloe(const std::string& id, CareStrategy* careStrategy,
         PlantState* initialState, const std::string& variety = "Vera");

    ~Aloe();
    
    std::string toString() const override;
};

#endif