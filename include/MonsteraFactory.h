#ifndef MONSTERA_FACTORY_H
#define MONSTERA_FACTORY_H

#include "OtherPlantFactory.h"

class CareScheduler;
class Plant;

class MonsteraFactory : public OtherPlantFactory {
public:
    virtual ~MonsteraFactory() {}
    virtual Plant* buildPlant(CareScheduler* scheduler= nullptr) const;
};

#endif // MONSTERA_FACTORY_H