#ifndef ROSE_H
#define ROSE_H

#include "Flower.h"

class Rose : public Flower {
public:
    Rose(const std::string& id, CareStrategy* careStrategy,
         PlantState* initialState, const std::string& roseColor,
         const std::string& type = "Hybrid Tea");

    ~Rose();

    std::string toString() const override;
};

#endif