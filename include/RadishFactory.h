#ifndef RADISH_FACTORY_H
#define RADISH_FACTORY_H

#include "VegetableFactory.h"

class RadishFactory : public VegetableFactory {
public:
    virtual ~RadishFactory() {}
    virtual Plant* buildPlant() const;
};

#endif // RADISH_FACTORY_H