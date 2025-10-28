/**
 * @file Decorator.h
 * @brief Defines the abstract Decorator base class for plant decorations
 * @author Gabriela Berimbau
 * @date 2025-10-26
 * 
 * This file contains the declaration of the Decorator abstract base class, which serves
 * as the foundation for the Decorator pattern implementation. It provides a common
 * interface for all concrete decorators that add features and enhancements to plant objects.
 * 
 * @see Plant
 * @see RibbonDecorator
 * @see GiftWrapDecorator
 * @see DecorativePotDecorator
 */

#ifndef DECORATOR_H
#define DECORATOR_H

#include <sstream>

#include "Plant.h"

/**
 * @class Decorator
 * @brief Abstract base class for all plant decorators
 * 
 * The Decorator class provides the abstract interface for the Decorator pattern
 * implementation. It wraps a Plant object and defines the contract that all concrete
 * decorators must follow. Decorators can be stacked to combine multiple enhancements
 * on a single plant, with each decorator adding its own cost and description modifications.
 * 
 * @note Part of the Decorator pattern implementation
 * @see Plant
 * @see RibbonDecorator
 * @see GiftWrapDecorator
 * @see DecorativePotDecorator
 */
class Decorator {
    public:
        /**
         * @brief Constructs a Decorator wrapping the specified plant
         * 
         * @param p Pointer to the Plant object to be decorated
         * @pre p must not be nullptr
         */
        Decorator(Plant* p);

        /**
         * @brief Virtual destructor for proper cleanup
         */
        virtual ~Decorator();

        /**
         * @brief Calculates total price including decoration cost
         * 
         * @return Total price of the decorated plant
         */
        virtual double getPrice() = 0;

        /**
         * @brief Returns description including decoration details
         * 
         * @return String description of the decorated plant
         */
        virtual std::string description() = 0;

    protected:
        /**
         * @brief Pointer to the wrapped plant object
         */
        Plant* plant;
};

#endif //DECORATOR_H