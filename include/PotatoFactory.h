#ifndef POTATO_FACTORY_H
#define POTATO_FACTORY_H

#include "VegetableFactory.h"

class PotatoFactory : public VegetableFactory {
public:
    virtual ~PotatoFactory() {}
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // POTATO_FACTORY_H