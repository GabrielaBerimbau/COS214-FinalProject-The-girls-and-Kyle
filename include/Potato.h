#ifndef POTATO_H
#define POTATO_H

#include "Vegetable.h"

class Potato : public Vegetable {
public:
    Potato(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety = "Russet",
           const std::string& skin = "brown");

    ~Potato();
    
    std::string toString() const override;
};

#endif