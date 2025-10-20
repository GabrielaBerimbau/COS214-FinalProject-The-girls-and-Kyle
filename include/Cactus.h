#ifndef CACTUS_H
#define CACTUS_H

#include "Succulent.h"

class Cactus : public Succulent {
public:
    Cactus();
    
    ~Cactus();
    
    void performCare() override;
};

#endif