// #include "Decorator.h"
#include "include/Decorator.h"


Decorator::Decorator(Plant *p) : plant(p)
{
}

Decorator::~Decorator()
{
    delete plant;
}