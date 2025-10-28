#include "include/DecorativePotDecorator.h"

DecorativePotDecorator::DecorativePotDecorator(Plant *plant, std::string colour) 
    : Decorator(plant), potColour(colour)
{
}

double DecorativePotDecorator::getPrice()
{
    if (plant) {
        return plant->getPrice() + POT_PRICE;
    }
    return 0.0;  // Add return for the case when plant is nullptr
}

std::string DecorativePotDecorator::description()
{
    if (plant) {
        std::ostringstream output;
        output << plant->description();
        output << "Decoration: " << potColour << " pot\n";

        return output.str();
    }
    return "";  // Add return for the case when plant is nullptr
}