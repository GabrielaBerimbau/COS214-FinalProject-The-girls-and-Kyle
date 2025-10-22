#ifndef ROSE_FACTORY_H
#define ROSE_FACTORY_H

#include "FlowerFactory.h"

class RoseFactory : public FlowerFactory {
public:
    virtual ~RoseFactory() {}
    virtual Plant* buildPlant() const;//returns new Rose
};

#endif // ROSE_FACTORY_H
