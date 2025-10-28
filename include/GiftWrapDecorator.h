/**
 * @file GiftWrapDecorator.h
 * @brief Defines the GiftWrapDecorator class for adding gift wrapping to plants
 * @author Gabriela Berimbau
 * @date 2025-10-26
 * 
 * This file contains the declaration of the GiftWrapDecorator class, which is a concrete
 * decorator in the Decorator pattern. It allows plants to be gift wrapped, adding both
 * visual appeal and additional cost to the base plant price.
 * 
 * @see Decorator
 * @see Plant
 */

#ifndef GIFT_WRAP_DECORATOR_H
#define GIFT_WRAP_DECORATOR_H

#include "Decorator.h"

/**
 * @class GiftWrapDecorator
 * @brief Concrete decorator that adds gift wrapping functionality to plants
 * 
 * The GiftWrapDecorator class extends the Decorator base class to provide gift wrapping
 * enhancement for plant objects. It adds a fixed cost to the wrapped plant's price and
 * modifies the description to indicate the gift wrapping feature. This decorator can be
 * combined with other decorators (e.g., RibbonDecorator, DecorativePotDecorator) to create
 * fully customized plant presentations.
 * 
 * @note Part of the Decorator pattern implementation
 * @see Decorator
 * @see RibbonDecorator
 * @see DecorativePotDecorator
 */
class GiftWrapDecorator : public Decorator {
    public:
        /**
         * @brief Constructs a GiftWrapDecorator wrapping the specified plant
         * 
         * @param plant Pointer to the Plant object to be gift wrapped
         * @pre plant must not be nullptr
         */
        GiftWrapDecorator(Plant* plant);

        /**
         * @brief Virtual destructor for proper cleanup
         */
        virtual ~GiftWrapDecorator() = default;

        /**
         * @brief Calculates total price including gift wrapping cost
         * 
         * @return Total price (wrapped plant price + GIFT_WRAP_PRICE)
         */
        double getPrice() const override;

        /**
         * @brief Returns description with gift wrapping notation
         * 
         * @return String in format: "[plant description] with gift wrapping"
         */
        std::string description() const override;

    private:
        /**
         * @brief Fixed cost for gift wrapping service
         */
        static constexpr double GIFT_WRAP_PRICE = 20.0;

};

#endif //GIFT_WRAP_DECORATOR_H      