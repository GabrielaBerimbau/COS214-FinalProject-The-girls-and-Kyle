// #include "DecorativePotDecorator.h"
#include "include/DecorativePotDecorator.h"

DecorativePotDecorator::DecorativePotDecorator(Plant *plant, std::string colour) : Decorator(plant), potColour(colour)
{
}

double DecorativePotDecorator::getPrice()
{
    if (plant) {
        return plant->getPrice() + POT_PRICE;
    }
}

std::string DecorativePotDecorator::description()
{
    if (plant) {
        std::ostringstream output;
        output << plant->description();
        output << "Decoration: " << potColour << " pot\n";

        return output.str();
    }
}
