/**
 * @file ConcreteIterator.h
 * @brief Defines the ConcreteIterator class for traversing order structures
 * @author Kyle McCalgan
 * @date 2025-10-26
 * 
 * This file contains the declaration of the ConcreteIterator class, which is a concrete
 * iterator in the Iterator pattern. It provides functionality to traverse through complex
 * order structures, collecting and iterating over all leaf items (individual plants) within
 * both simple and composite orders.
 * 
 * @see Iterator
 * @see Order
 * @see ConcreteOrder
 * @see Leaf
 */

#ifndef CONCRETEITERATOR_H
#define CONCRETEITERATOR_H

#include "Iterator.h"
#include <vector>

class Order;

/**
 * @class ConcreteIterator
 * @brief Concrete iterator for traversing order component structures
 * 
 * The ConcreteIterator class extends the Iterator base class to provide traversal
 * functionality for order structures in the Composite pattern. It flattens the
 * hierarchical order structure by collecting all leaf items (individual plant orders)
 * into a linear collection, making it easy to iterate through all items regardless
 * of the composite structure depth.
 * 
 * @note Part of the Iterator pattern implementation working with Composite pattern
 * @see Iterator
 * @see Order
 * @see ConcreteOrder
 * @see Leaf
 */
class ConcreteIterator : public Iterator {
    private:
        /**
         * @brief Collection of order items (leaves) for iteration
         */
        std::vector<Order*> items;

        /**
         * @brief Current position in the iteration sequence
         */
        int currentIndex;

        /**
         * @brief Recursively collects all leaf items from the order structure
         * 
         * Helper method that traverses the composite order structure and extracts
         * all leaf nodes (individual plant items) into the items vector.
         * 
         * @param order Root order node to collect leaves from
         */
        void collectLeaves(Order* order);
        
    public:
        /**
         * @brief Constructs a ConcreteIterator for the specified order structure
         * 
         * @param root Pointer to the root Order object to iterate over
         * @pre root must not be nullptr
         */    
        explicit ConcreteIterator(Order* root);

        /**
         * @brief Virtual destructor for proper cleanup
         */        
        virtual ~ConcreteIterator();

        
        /**
         * @brief Resets iterator to the first element
         */
        void first() override;

        /**
         * @brief Advances iterator to the next element
         */
        void next() override;

        /**
         * @brief Checks if iteration has reached the end
         * 
         * @return True if no more elements to iterate, false otherwise
         */
        bool isDone() const override;

        /**
         * @brief Returns the current order item in the iteration
         * 
         * @return Pointer to the current Order item, or nullptr if iteration is done
         */        
        Order* currentItem() const override;
    };

#endif // CONCRETEITERATOR_H