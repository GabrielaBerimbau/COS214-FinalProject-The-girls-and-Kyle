#ifndef SUCCULANT_FACTORY_H
#define SUCCULANT_FACTORY_H

#include "OtherPlantFactory.h"

class SucculantFactory : public OtherPlantFactory {
public:
    virtual ~SucculantFactory() {}
    virtual Plant* buildPlant() const = 0;
};

#endif // SUCCULANT_FACTORY_H