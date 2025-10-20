#ifndef DECORATOR_H
#define DECORATOR_H

#include "Plant.h"

class Decorator {
    public:
        Decorator(Plant* p);
        virtual ~Decorator();

    protected:
        Plant* plant;
};

#endif //DECORATOR_H