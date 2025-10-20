#ifndef ALOE_FACTORY_H
#define ALOE_FACTORY_H

#include "SucculantFactory.h"

class AloeFactory : public SucculantFactory {
public:
    virtual ~AloeFactory() {}
    virtual Plant* buildPlant() const;
};

#endif // ALOE_FACTORY_H