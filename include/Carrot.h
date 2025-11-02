#ifndef POTATO_H
#define POTATO_H

#include "Vegetable.h"

class Carrot : public Vegetable {
public:
    Carrot(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety = "Imperator",
           const std::string& skin = "orange");

    ~Carrot();
    
    std::string toString() const override;
};

#endif