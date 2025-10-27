#ifndef CACTUS_FACTORY_H
#define CACTUS_FACTORY_H

#include "SucculentFactory.h"

class CactusFactory : public SucculantFactory {
public:
    virtual ~CactusFactory() {}
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // CACTUS_FACTORY_H