#ifndef VENUS_FLY_TRAP_FACTORY_H
#define VENUS_FLY_TRAP_FACTORY_H

#include "OtherPlantFactory.h"

class VenusFlyTrapFactory : public OtherPlantFactory {
public:
    virtual ~VenusFlyTrapFactory() {}
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // VENUS_FLY_TRAP_FACTORY_H
