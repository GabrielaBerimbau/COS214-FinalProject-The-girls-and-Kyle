#ifndef Carrot_FACTORY_H
#define Carrot_FACTORY_H

#include "VegetableFactory.h"

class CarrotFactory : public VegetableFactory {
public:
    virtual ~CarrotFactory() {}
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // Carrot_FACTORY_H