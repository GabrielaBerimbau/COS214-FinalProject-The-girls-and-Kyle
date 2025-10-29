/**
 * @file AbstractFinalOrder.h
 * @author Gabi de Gouevia
 * @brief Declares the AbstractFinalOrder interface, serving as the base for Prototype pattern implementations.
 * 
 * This abstract class defines the essential interface for all concrete FinalOrder types in the system.
 * It enforces implementation of core functionality such as cloning, total calculation, and summary generation.
 */
#ifndef ABSTRACTFINALORDER_H
#define ABSTRACTFINALORDER_H

#include <vector>
#include <string>
#include "Order.h"

/**
 * @class AbstractFinalOrder
 * @brief Abstract base class defining the interface for final orders.
 * 
 * This class forms the foundation of the **Prototype Pattern** implementation.
 * It provides pure virtual functions for cloning, calculating total price,
 * adding sub-orders, and generating textual summaries.
 */

class AbstractFinalOrder {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of derived classes.
     */
    virtual ~AbstractFinalOrder() = default;

    /**
     * @brief Creates a deep copy (clone) of this order.
     * 
     * This function must be implemented by all subclasses to support 
     * cloning in the Prototype pattern.
     * 
     * @return A pointer to the cloned AbstractFinalOrder object.
     */
    virtual AbstractFinalOrder* clone() const = 0;

    /**
     * @brief Calculates the total price of all orders.
     * 
     * Must return the combined cost of all associated order items or sub-orders.
     * 
     * @return The total price as a double.
     */
    virtual double calculateTotalPrice() const = 0;

    /**
     * @brief Adds a new sub-order to the final order.
     * 
     * Used to build composite orders from multiple `Order` objects.
     * 
     * @param order Pointer to the `Order` object to be added.
     */
    virtual void addOrder(Order* order) = 0;

    /**
     * @brief Generates a summary of the entire final order.
     * 
     * Must return a string containing formatted details of all orders 
     * and their respective prices.
     * 
     * @return A string summary of the order.
     */
    virtual std::string getSummary() const = 0;
};

#endif
