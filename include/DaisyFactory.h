#ifndef DAISY_FACTORY_H
#define DAISY_FACTORY_H

#include "FlowerFactory.h"

class DaisyFactory : public FlowerFactory {
public:
    virtual ~DaisyFactory() {}
    virtual Plant* buildPlant() const;   // returns new Daisy
};

#endif // DAISY_FACTORY_H