#ifndef CACTUS_FACTORY_H
#define CACTUS_FACTORY_H

#include "SucculantFactory.h"

class CactusFactory : public SucculantFactory {
public:
    virtual ~CactusFactory() {}
    virtual Plant* buildPlant() const;
};

#endif // CACTUS_FACTORY_H