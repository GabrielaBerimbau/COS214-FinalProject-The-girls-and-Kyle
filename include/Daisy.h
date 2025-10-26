#ifndef DAISY_H
#define DAISY_H

#include "Flower.h"

class Daisy : public Flower {
public:
    Daisy(const std::string& id, CareStrategy* careStrategy,
          PlantState* initialState, const std::string& petalColor,
          const std::string& varietyType = "Common");
    
    ~Daisy();
    
    std::string toString() const override;
};

#endif