#ifndef OTHER_PLANT_FACTORY_H
#define OTHER_PLANT_FACTORY_H

#include "PlantFactory.h"

class OtherPlantFactory : public PlantFactory {
public:
    virtual ~OtherPlantFactory() {}
    virtual Plant* buildPlant() const = 0;
};

#endif // OTHER_PLANT_FACTORY_H