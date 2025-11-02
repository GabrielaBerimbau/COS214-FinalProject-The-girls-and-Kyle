/**
 * @file Leaf.h
 * @brief Defines the Leaf class representing individual plant items in orders
 * @author Gabriela Berimbau
 * @date 2025-10-27
 * 
 * This file contains the declaration of the Leaf class, which is a leaf component
 * in the Composite pattern. It represents a single physical plant item in an order,
 * wrapping an actual Plant object from inventory. Multiple Leaf objects are used to
 * represent multiple individual plants in an order structure.
 * 
 * @see Order
 * @see Plant
 * @see ConcreteOrder
 */
#ifndef LEAF_H
#define LEAF_H

#include "Order.h"
#include "Plant.h"

#include <string>
#include <iostream>

class Iterator;

/**
 * @class Leaf
 * @brief Leaf component representing a single physical plant in an order
 * 
 * The Leaf class extends the Order base class to represent individual plant items
 * in the Composite pattern. Each Leaf wraps one Plant object (plain or decorated)
 * from inventory. Leaf nodes are terminal nodes with no children, representing
 * actual physical plants that customers purchase.
 * 
 * @note Part of the Composite pattern implementation as the leaf component
 * @see Order
 * @see Plant
 * @see ConcreteOrder
 * @see Decorator
 */
class Leaf : public Order {
    public:
        /**
         * @brief Constructs a Leaf wrapping a single physical plant
         * 
         * @param p Pointer to the Plant object from inventory (may be decorated)
         * @param ownsPlant Whether this Leaf owns and should delete the plant on destruction
         * @pre p must not be nullptr
         */
        Leaf(Plant* p, bool ownsPlant = true);

        /**
         * @brief Virtual destructor for proper cleanup
         * 
         * Deletes the wrapped plant if ownsPlant is true.
         */        
        virtual ~Leaf();


        /**
         * @brief Returns the price of the wrapped plant
         *
         * @return Price of the single plant (including any decorations)
         */
        virtual double getPrice() const override;

        /**
         * @brief Returns the description of the wrapped plant
         * 
         * @return String description of the plant
         */        
        virtual std::string description() override;


        /**
         * @brief Cannot add children to a leaf node
         * 
         * Leaf nodes cannot have children in the composite structure.
         * Prints an error message or throws an exception.
         * 
         * @param order Order component (ignored)
         */
        virtual void add(Order* order) override;

        /**
         * @brief Cannot remove children from a leaf node
         * 
         * Leaf nodes cannot have children in the composite structure.
         * Prints an error message or throws an exception.
         * 
         * @param order Order component (ignored)
         */        
        virtual void remove(Order* order) override;
        

        /**
         * @brief Creates a deep copy of this leaf and its wrapped plant
         * 
         * @return Pointer to the cloned Leaf with cloned plant
         */
        Order* clone() const override;

        /**
         * @brief Returns the name of the wrapped plant
         * 
         * @return Plant name string
         */        
        std::string getName() const override;
        
        /**
         * @brief Creates an iterator for this leaf node
         *
         * @return Pointer to newly created Iterator for this leaf
         */
        virtual Iterator* createIterator() override;

        /**
         * @brief Prints the structure of this leaf node
         *
         * Prints this leaf item with proper indentation.
         *
         * @param indent Indentation level for nested structures
         * @param prefix String prefix for tree-style formatting
         */
        virtual void printStructure(int indent = 0, const std::string& prefix = "") const override;

    private:
        /**
         * @brief The single physical plant this Leaf represents
         */
        Plant* plant;  

        /**
         * @brief Whether this Leaf owns and should delete the plant on destruction
         */
        bool ownsPlant;
        
};

#endif //LEAF_H