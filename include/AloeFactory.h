#ifndef ALOE_FACTORY_H
#define ALOE_FACTORY_H

#include "SucculentFactory.h"

class AloeFactory : public SucculantFactory {
public:
    virtual ~AloeFactory() {}
    
    virtual Plant* buildPlant() const;
};

#endif // ALOE_FACTORY_H