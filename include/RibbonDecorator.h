/**
 * @file RibbonDecorator.h
 * @brief Defines the RibbonDecorator class for adding ribbon decoration to plants
 * @author Gabriela Berimbau
 * @date 2025-10-26
 * 
 * This file contains the declaration of the RibbonDecorator class, which is a concrete
 * decorator in the Decorator pattern. It allows plants to be decorated with ribbons,
 * adding both aesthetic appeal and additional cost to the base plant price.
 * 
 * @see Decorator
 * @see Plant
 */

#ifndef RIBBON_DECORATOR_H
#define RIBBON_DECORATOR_H

#include "Decorator.h"

/**
 * @class RibbonDecorator
 * @brief Concrete decorator that adds ribbon decoration functionality to plants
 * 
 * The RibbonDecorator class extends the Decorator base class to provide ribbon
 * decoration enhancement for plant objects. It adds a fixed cost to the decorated
 * plant's price and modifies the description to indicate the ribbon feature. This
 * decorator can be combined with other decorators (e.g., GiftWrapDecorator,
 * DecorativePotDecorator) to create fully customized plant presentations.
 * 
 * @note Part of the Decorator pattern implementation
 * @see Decorator
 * @see GiftWrapDecorator
 * @see DecorativePotDecorator
 */
class RibbonDecorator : public Decorator { //inherits from decorator
    public:
        /**
         * @brief Constructs a RibbonDecorator wrapping the specified plant
         * 
         * @param plant Pointer to the Plant object to be decorated with ribbon
         * @pre plant must not be nullptr
         */
        RibbonDecorator(Plant* plant);

        /**
         * @brief Virtual destructor for proper cleanup
         */
        virtual ~RibbonDecorator() = default;

        /**
         * @brief Calculates total price including ribbon decoration cost
         * 
         * @return Total price (wrapped plant price + RIBBON_PRICE)
         */
        double getPrice() const override;

        /**
         * @brief Returns description with ribbon decoration notation
         * 
         * @return String in format: "[plant description] with ribbon"
         */
        std::string description() const override;


    private: 
        /**
         * @brief Fixed cost for ribbon decoration service
         */
        static constexpr double RIBBON_PRICE = 15.0;

};

#endif //RIBBON_DECORATOR_H