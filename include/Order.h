/**
 * @file Order.h
 * @brief Defines the abstract Order interface for the Composite pattern
 * @author Gabriela Berimbau
 * @date 2025-10-27
 * 
 * This file contains the declaration of the Order abstract base class, which serves
 * as the component interface in the Composite pattern. It defines the common interface
 * for both leaf components (individual plant items) and composite components (order
 * groups), enabling uniform treatment of simple and complex order structures.
 * 
 * @see Leaf
 * @see ConcreteOrder
 * @see Iterator
 */

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>

class Iterator;

/**
 * @class Order
 * @brief Abstract component interface for the Composite pattern in order structures
 * 
 * The Order class provides the abstract interface for the Composite pattern
 * implementation. It defines operations common to both Leaf (individual plants) and
 * ConcreteOrder (composite groups) components, allowing clients to treat individual
 * plants and complex order hierarchies uniformly. This enables building sophisticated
 * order structures like gift baskets, bulk orders, and wedding packages.
 * 
 * @note Part of the Composite pattern implementation as the component interface
 * @see Leaf
 * @see ConcreteOrder
 * @see Iterator
 */
class Order { //composite pattern - Component
    public:
        /**
         * @brief Virtual destructor for proper cleanup
         */    
        virtual ~Order() = default;

        /**
         * @brief Calculates the total price of this order component
         *
         * For Leaf: returns the price of the single plant (including decorations)
         * For ConcreteOrder: returns sum of all children's prices
         *
         * @return Total price of this order component
         */
        virtual double getPrice() const = 0;

        /**
         * @brief Generates a description of this order component
         * 
         * For Leaf: returns the plant description
         * For ConcreteOrder: returns combined descriptions of all children
         * 
         * @return String description of this order component
         */
        virtual std::string description() = 0;


        /**
         * @brief Adds a child order component
         * 
         * For Leaf: prints error or throws exception (leaves cannot have children)
         * For ConcreteOrder: adds the order to the children collection
         * 
         * @param order Pointer to the Order component to add
         */
        virtual void add(Order* order) = 0;

        /**
         * @brief Removes a child order component
         * 
         * For Leaf: prints error or throws exception (leaves cannot have children)
         * For ConcreteOrder: removes the order from the children collection
         * 
         * @param order Pointer to the Order component to remove
         */        
        virtual void remove(Order* order) = 0;
        // virtual Order* getChild(int index) = 0; //only for a composite

        /**
         * @brief Creates a deep copy of this order component
         * 
         * Clones this order and recursively clones all children if composite.
         * 
         * @return Pointer to the cloned Order component
         */
        virtual Order* clone() const = 0;

        /**
         * @brief Returns the name of this order component
         * 
         * For Leaf: returns the plant name
         * For ConcreteOrder: returns the order group name
         * 
         * @return Name or label for this order component
         */
        virtual std::string getName() const = 0;

        /**
         * @brief Creates an iterator for traversing this order structure
         *
         * @return Pointer to newly created Iterator for this order component
         */
        virtual Iterator* createIterator() = 0;

        /**
         * @brief Prints the hierarchical structure of this order component
         *
         * Recursively prints the order structure showing the hierarchy of
         * composite orders and their leaf items with proper indentation.
         *
         * @param indent Indentation level for nested structures (default: 0)
         * @param prefix String prefix for tree-style formatting
         */
        virtual void printStructure(int indent = 0, const std::string& prefix = "") const = 0;
};

#endif

