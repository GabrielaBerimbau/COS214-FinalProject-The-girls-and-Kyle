#ifndef ALOE_H
#define ALOE_H

#include "Succulent.h"

class Aloe : public Succulent {
public:
    Aloe();
    
    ~Aloe();
    
    void performCare() override;
};

#endif