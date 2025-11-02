#include "include/GiftWrapDecorator.h"

GiftWrapDecorator::GiftWrapDecorator(Plant *plant) : Decorator(plant) {
}

double GiftWrapDecorator::getPrice() const {
    if (plant) {
        return plant->getPrice() + GIFT_WRAP_PRICE;
    }
    return GIFT_WRAP_PRICE;
}

std::string GiftWrapDecorator::description() const {
    if (plant) {
        std::ostringstream output;
        output << plant->description();
        output << "Decoration: gift wrapping\n";
        return output.str();
    }
    return "Decoration: gift wrapping\n";
}