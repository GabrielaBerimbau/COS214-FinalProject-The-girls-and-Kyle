#ifndef VEGETABLE_FACTORY_H
#define VEGETABLE_FACTORY_H

#include "PlantFactory.h"

class VegetableFactory : public PlantFactory {
public:
    virtual ~VegetableFactory() {}
    virtual Plant* buildPlant() const = 0;
};

#endif // VEGETABLE_FACTORY_H