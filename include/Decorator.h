/**
 * @file Decorator.h
 * @brief Defines the abstract Decorator base class for plant decorations
 * @author Gabriela Berimbau
 * @date 2025-10-26 
 * 
 * This file contains the declaration of the Decorator abstract base class, which serves
 * as the foundation for the Decorator pattern implementation. It inherits from Plant and
 * wraps another Plant object, providing a common interface for all concrete decorators
 * that add features and enhancements to plant objects.
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
class Decorator : public Plant {
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
        virtual double getPrice() const override;

        /**
         * @brief Returns description including decoration details
         * 
         * Overrides Plant's description() method to append decoration information
         * to the wrapped plant's description.
         * 
         * @return String description of the decorated plant
         */
        virtual std::string description() const override;

        /**
         * @brief Performs daily update on the wrapped plant
         * 
         * Delegates the daily update operation to the wrapped plant, ensuring
         * that decorated plants maintain their lifecycle and care requirements.
         */
        void dailyUpdate() override;

        /**
         * @brief Returns string representation of the decorated plant
         * 
         * Delegates to the wrapped plant's toString() method to maintain
         * consistent string representation throughout the decoration chain.
         * 
         * @return String representation of the plant
         */
        std::string toString() const override;

        /**
         * @brief Strips all decorators and returns the base plant
         * 
         * Recursively unwraps the decorator chain to find and return the
         * underlying base plant object. The decorators are NOT deleted,
         * only the pointer to the base plant is returned.
         * 
         * @return Pointer to the base Plant (not a Decorator)
         */
        static Plant* stripDecorations(Plant* plant);

        /**
         * @brief Checks if a plant is decorated
         * 
         * @param plant Pointer to plant to check
         * @return true if plant is wrapped in any decorator, false otherwise
         */
        static bool isDecorated(Plant* plant);

        /**
         * @brief Gets the base plant without deleting decorators
         * 
         * Traverses the decorator chain to find the base plant.
         * Does NOT take ownership or delete anything.
         * 
         * @return Pointer to base plant
         */
        Plant* getBasePlant();

        /**
         * @brief Gets the wrapped plant (may be another decorator or base plant)
         * 
         * @return Pointer to the directly wrapped plant
         */
        Plant* getWrappedPlant() const;

    protected:
        /**
         * @brief Pointer to the wrapped plant object
         */
        Plant* plant;
};

#endif //DECORATOR_H