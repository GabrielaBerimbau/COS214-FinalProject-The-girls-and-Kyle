#ifndef VEGETABLE_FACTORY_H
#define VEGETABLE_FACTORY_H

#include "PlantFactory.h"

class VegetableFactory : public PlantFactory {
public:
    virtual ~VegetableFactory() {}
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const = 0;
};

#endif // VEGETABLE_FACTORY_H