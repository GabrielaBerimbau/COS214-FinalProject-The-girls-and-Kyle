#ifndef FLOWER_FACTORY_H
#define FLOWER_FACTORY_H

#include "PlantFactory.h"

class FlowerFactory : public PlantFactory {
public:
    virtual ~FlowerFactory() {}
    // still abstract concrete flowers will override buildPlant()
    virtual Plant* buildPlant() const = 0;
};

#endif // FLOWER_FACTORY_H