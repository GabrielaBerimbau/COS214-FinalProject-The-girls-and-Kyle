#ifndef STRELITZIA_FACTORY_H
#define STRELITZIA_FACTORY_H

#include "FlowerFactory.h"

class StrelitziaFactory : public FlowerFactory {
public:
    virtual ~StrelitziaFactory() {}
    virtual Plant* buildPlant(CareScheduler* scheduler=nullptr) const;
};

#endif // STRELITZIA_FACTORY_H

