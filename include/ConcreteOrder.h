/**
 * @file ConcreteOrder.h
 * @brief Defines the ConcreteOrder class for composite order structures
 * @author Gabriela Berimbau & Kyle McCalgan
 * @date 2025-10-28
 * 
 * This file contains the declaration of the ConcreteOrder class, which is a composite
 * component in the Composite pattern. It represents complex orders that can contain
 * multiple order items (Leaf objects) or other nested ConcreteOrder objects, allowing
 * for hierarchical order structures such as gift baskets, bulk orders, or wedding packages.
 * 
 * @see Order
 * @see Leaf
 * @see Iterator
 */
#ifndef CONCRETEORDER_H
#define CONCRETEORDER_H

#include "Order.h"
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
class Iterator; 

/**
 * @class ConcreteOrder
 * @brief Composite component that can contain multiple order items or nested orders
 * 
 * The ConcreteOrder class extends the Order base class to provide composite functionality
 * in the Composite pattern. It can hold a collection of Order objects (either Leaf items
 * representing individual plants or other ConcreteOrder objects), enabling the creation
 * of complex hierarchical order structures. ConcreteOrder calculates its total price by
 * recursively summing all children's prices and builds descriptions by combining all
 * children's descriptions.
 * 
 * @note Part of the Composite pattern implementation
 * @see Order
 * @see Leaf
 * @see Iterator
 * @see ConcreteIterator
 */
class ConcreteOrder : public Order {
    public:
        /**
         * @brief Constructs a ConcreteOrder with the specified name
         * 
         * @param orderN Name for this order group (e.g., "Wedding Package", "Gift Basket")
         */ 
        ConcreteOrder(std::string orderN);

        /**
         * @brief Virtual destructor for proper cleanup
         */
        virtual ~ConcreteOrder();


        /**
         * @brief Calculates total price of all children in this composite order
         * 
         * Recursively sums the prices of all child orders (both Leaf items and
         * nested ConcreteOrders) to determine the total price of this composite.
         * 
         * @return Total price as sum of all children's prices
         */
        virtual double getPrice() override;

        /**
         * @brief Generates description including all children's descriptions
         * 
         * Builds a description string by combining the order name
         * with descriptions from all child orders in the composite structure.
         * 
         * @return String description in format: "[orderName]: [child descriptions]"
         */        
        virtual std::string description() override;


        /**
         * @brief Adds a child order component to this composite
         * 
         * @param order Pointer to the Order object (Leaf or ConcreteOrder) to add
         * @pre order must not be nullptr
         */        
        virtual void add(Order* order) override;

        /**
         * @brief Removes a child order component from this composite
         * 
         * @param order Pointer to the Order object to remove
         */        
        virtual void remove(Order* order) override;
        // virtual Order* getChild(int index) override; //only for a composite

        /**
         * @brief Creates a deep copy of this composite order
         * 
         * Clones this ConcreteOrder and recursively clones all child orders.
         * 
         * @return Pointer to the cloned ConcreteOrder with all children cloned
         */
        virtual Order* clone() const override;

        /**
         * @brief Returns the name of this composite order
         * 
         * @return Order name string
         */        
        virtual std::string getName() const override;

        /**
         * @brief Creates an iterator for traversing this order structure
         * 
         * Creates and returns a ConcreteIterator that can traverse all leaf
         * items within this composite order structure.
         * 
         * @return Pointer to newly created Iterator for this order
         */        
        virtual Iterator* createIterator() override;
        
        /**
         * @brief Returns the collection of child orders in this composite
         * 
         * Provides access to the internal vector of child Order objects for
         * iteration and traversal purposes.
         * 
         * @return Vector containing pointers to all child Order objects
         */        
        std::vector<Order*> getChildren() const;

    private:
        /**
         * @brief Collection of child order components (Leaf items or ConcreteOrders)
         */
        std::vector<Order*> plantList;

        /**
         * @brief Name identifying this composite order group
         */        
        std::string orderName;
        // std::string customerName; - in Person class
        // double totalPrice; - in final order
};

#endif //CONCRETEORDER_H

