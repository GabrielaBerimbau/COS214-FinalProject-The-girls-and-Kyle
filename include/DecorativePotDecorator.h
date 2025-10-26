/**
 * @file DecorativePotDecorator.h
 * @brief Defines the DecorativePotDecorator class for adding decorative pots to plants
 * @author Gabriela Berimbau
 * @date 2025-10-26
 * 
 * This file contains the declaration of the DecorativePotDecorator class, which is a concrete
 * decorator in the Decorator pattern. It allows plants to be placed in decorative pots with
 * customizable colors, adding both visual appeal and additional cost to the base plant price.
 * 
 * @see Decorator
 * @see Plant
 */

#ifndef DECORATIVE_POT_DECORATOR_H
#define DECORATIVE_POT_DECORATOR_H

#include "Decorator.h"

/**
 * @class DecorativePotDecorator
 * @brief Concrete decorator that adds decorative pot functionality to plants
 * 
 * The DecorativePotDecorator class extends the Decorator base class to provide decorative
 * pot enhancement for plant objects. It adds a fixed cost to the decorated plant's price
 * and modifies the description to indicate both the decorative pot feature and its color.
 * This decorator can be combined with other decorators (e.g., RibbonDecorator,
 * GiftWrapDecorator) to create fully customized plant presentations.
 * 
 * @note Part of the Decorator pattern implementation
 * @see Decorator
 * @see RibbonDecorator
 * @see GiftWrapDecorator
 */
class DecorativePotDecorator : public Decorator {
    public:
        /**
         * @brief Constructs a DecorativePotDecorator wrapping the specified plant
         * 
         * @param plant Pointer to the Plant object to be placed in a decorative pot
         * @param colour The color of the decorative pot
         * @pre plant must not be nullptr
         */
        DecorativePotDecorator(Plant* plant, std::string colour);

        /**
         * @brief Virtual destructor for proper cleanup
         */
        virtual ~DecorativePotDecorator() = default;
        
        /**
         * @brief Calculates total price including decorative pot cost
         * 
         * @return Total price (wrapped plant price + POT_PRICE)
         */
        double getPrice() override;

        /**
         * @brief Returns description with decorative pot notation including color
         * 
         * @return String in format: "[plant description] in [color] pot"
         */
        std::string description() override;

    private:
        /**
         * @brief Fixed cost for decorative pot service
         */ 
        static const double POT_PRICE = 80.0;

        /**
         * @brief Color of the decorative pot
         */
        std::string potColour;

};

#endif //DECORATIVE_POT_DECORATOR_H