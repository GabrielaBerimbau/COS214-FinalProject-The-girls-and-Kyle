#include "include/RibbonDecorator.h"

RibbonDecorator::RibbonDecorator(Plant *plant) : Decorator(plant) {
}

double RibbonDecorator::getPrice() const {
    if (plant) {
        return plant->getPrice() + RIBBON_PRICE;
    }
    return RIBBON_PRICE;
}

std::string RibbonDecorator::description() const {
    if (plant) {
        std::ostringstream output;
        output << plant->description();
        output << "Decoration: ribbon\n";
        return output.str();
    }
    return "Decoration: ribbon\n";
}