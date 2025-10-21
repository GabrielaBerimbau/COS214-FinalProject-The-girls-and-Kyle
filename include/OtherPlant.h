#ifndef OTHERPLANT_H
#define OTHERPLANT_H

#include "Plant.h"

class OtherPlant : public Plant {
public:
    OtherPlant();
    
    virtual ~OtherPlant();
    
    void performCare() override;
};

#endif