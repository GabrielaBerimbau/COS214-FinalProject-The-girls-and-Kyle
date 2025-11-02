/**
 * @file Iterator.h
 * @brief Defines the abstract Iterator interface for traversing order structures
 * @author Kyle McCalgan
 * @date 2025-10-26
 * 
 * This file contains the declaration of the Iterator abstract base class, which serves
 * as the foundation for the Iterator pattern implementation. It provides a common
 * interface for traversing through order component structures in the Composite pattern,
 * allowing sequential access to elements without exposing the underlying representation.
 * 
 * @see ConcreteIterator
 * @see Order
 * @see ConcreteOrder
 */

#ifndef ITERATOR_H
#define ITERATOR_H

class Order;

/**
 * @class Iterator
 * @brief Abstract interface for iterating through order collections
 * 
 * The Iterator class provides the abstract interface for the Iterator pattern
 * implementation. It defines the standard operations for traversing order structures,
 * allowing clients to access order elements sequentially without exposing the internal
 * composite structure. Concrete iterators implement this interface to provide specific
 * traversal strategies for different order compositions.
 * 
 * @note Part of the Iterator pattern implementation working with Composite pattern
 * @see ConcreteIterator
 * @see Order
 * @see ConcreteOrder
 * @see Leaf
 */
class Iterator {
public:
    /**
     * @brief Virtual destructor for proper cleanup
     */
    virtual ~Iterator() {}

    /**
     * @brief Resets iterator to the first element in the collection
     */
    virtual void first() = 0;

    /**
     * @brief Advances iterator to the next element in the collection
     */    
    virtual void next() = 0;

    /**
     * @brief Checks if the iteration has reached the end
     * 
     * @return True if no more elements to iterate, false otherwise
     */    
    virtual bool isDone() const = 0;

    /**
     * @brief Returns the current order element in the iteration
     * 
     * @return Pointer to the current Order object, or nullptr if iteration is done
     */    
    virtual Order* currentItem() const = 0;
};

#endif // ITERATOR_H