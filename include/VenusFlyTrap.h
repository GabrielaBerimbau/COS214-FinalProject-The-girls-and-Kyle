#ifndef VENUSFLYTRAP_H
#define VENUSFLYTRAP_H

#include "OtherPlant.h"

class VenusFlyTrap : public OtherPlant {
public:
    VenusFlyTrap();
    
    ~VenusFlyTrap();
    
    void performCare() override;
};

#endif