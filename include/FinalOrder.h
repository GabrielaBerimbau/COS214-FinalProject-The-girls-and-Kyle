/**
 * @file FinalOrder.h
 * @author Gabi de Gouevia
 * @brief Concrete Prototype representing a completed order that can be cloned.
 * 
 * * The FinalOrder class implements the **Prototype Pattern**, allowing deep cloning of
 * completed orders, including all nested Order objects. It also integrates the
 * **Iterator Pattern** to traverse composite orders efficiently when generating
 * summaries and invoices.
 */

#ifndef FINALORDER_H
#define FINALORDER_H

#include "AbstractFinalOrder.h"
#include "Order.h"
#include <vector>
#include <string>
#include <iostream>

/**
 * @class FinalOrder
 * @brief Implements the Prototype pattern to allow order duplication.
 *  * This class maintains a collection of `Order` objects and provides methods for
 * cloning, calculating total prices, and generating formatted order
 * summaries. It interacts closely with the **Iterator** to access sub-orders
 * within complex order hierarchies.
 */
class FinalOrder : public AbstractFinalOrder {
private:
    std::vector<Order*> orderList;  ///< Collection of all orders in this final order.
    std::string customerName;       ///< Customer's name.
    double totalPrice;              ///< Cached total price.

public:
    /**
     * @brief Constructs a new FinalOrder for the given customer.
     * @param name The name of the customer placing the order.
     */
    FinalOrder(const std::string& name);

    /**
     * @brief Copy constructor used for cloning.
     * @param other The FinalOrder to be cloned.
     */
    FinalOrder(const FinalOrder& other);

    /**
     * @brief Destructor for releasing Order pointers.
     * Ensures all owned `Order` pointers are properly deleted to prevent memory leaks.
     */
    ~FinalOrder() override;

    /**
     * @brief Creates a deep copy (clone) of this FinalOrder.
     * 
     * Uses the Prototype pattern to replicate the full structure and contents of
     * an existing order, including all sub-orders.
     * 
     * @return A pointer to the newly cloned FinalOrder.
     */
    FinalOrder* clone() const override;

    /**
     * @brief Adds a new order to the final order list.
     * 
     * @param order Pointer to the `Order` object being added.
     */
    void addOrder(Order* order) override;

    /**
     * @brief Calculates the total price of all contained orders.
     * 
     * Uses an Iterator to traverse composite order structures when necessary.
     * 
     * @return The total cost of all items in the order.
     */
    double calculateTotalPrice() const override;

    /**
     * @brief Generates a formatted textual summary of the order.
     * 
     * Iterates through all nested orders and items to produce a complete overview.
     * 
     * @return A detailed string summary including all order names and totals.
     */
    std::string getSummary() const override;

    /**
     * @brief Prints a structured invoice for this order to the console.
     *
     * Uses the Iterator pattern to list all items in nested orders,
     * along with their individual and total prices.
     */
    void printInvoice() const;

    /**
     * @brief Prints the complete hierarchical structure of this FinalOrder.
     *
     * Displays the entire order structure showing all nested orders and items
     * with proper indentation and visual tree formatting. This provides a
     * clear visualization of the composite order hierarchy.
     */
    void printOrderStructure() const;

    /**
     * @brief Generates a formatted hierarchical receipt string.
     *
     * Creates a detailed receipt showing the order structure with all
     * suborders and their items properly formatted and indented. This
     * is specifically designed for GUI display in the receipt screen.
     *
     * @return A formatted string containing the complete order hierarchy
     */
    std::string getFormattedReceipt() const;

private:
    /**
     * @brief Helper function to recursively build order hierarchy strings.
     *
     * Recursively traverses the order structure and builds a formatted
     * string representation with proper indentation.
     *
     * @param order The order to format
     * @param indent The current indentation level
     * @param isLast Whether this is the last item in the current level
     * @return A formatted string for this order and its children
     */
    std::string formatOrderRecursive(Order* order, int indent, bool isLast) const;
};

#endif
