// #include "RibbonDecorator.h"
#include "include/RibbonDecorator.h"


RibbonDecorator::RibbonDecorator(Plant *plant) : Decorator(plant)
{
}

double RibbonDecorator::getPrice()
{
    if (plant) {
        return plant->getPrice() + RIBBON_PRICE;
    }
}

std::string RibbonDecorator::getDescription()
{
    if (plant) {
        std::ostringstream output;
        output << plant->description();
        output << "Decoration: ribbon\n";

        return output.str();
    }
}
