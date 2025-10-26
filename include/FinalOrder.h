/**
 * @file FinalOrder.h
 * @brief Concrete Prototype representing a completed order that can be cloned.
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
 */
class FinalOrder : public AbstractFinalOrder {
private:
    std::vector<Order*> orderList;  ///< List of Order objects in this final order.
    std::string customerName;       ///< Customer's name.
    double totalPrice;              ///< Cached total price.

public:
    /**
     * @brief Constructor for creating a new FinalOrder.
     * @param name Customer name.
     */
    FinalOrder(const std::string& name);

    /**
     * @brief Copy constructor used for cloning.
     * @param other The FinalOrder to be cloned.
     */
    FinalOrder(const FinalOrder& other);

    /**
     * @brief Destructor for releasing Order pointers.
     */
    ~FinalOrder();

    /**
     * @brief Clones this FinalOrder object (Prototype pattern).
     * @return A deep copy of the FinalOrder.
     */
    FinalOrder* clone() const override;

    /**
     * @brief Adds an Order item to the final order.
     * @param order Pointer to an Order.
     */
    void addOrder(Order* order) override;

    /**
     * @brief Calculates total price of all Order items.
     * @return Total cost as a double.
     */
    double calculateTotalPrice() const override;

    /**
     * @brief Returns a short text summary of the order contents.
     * @return A string summary listing the items and total.
     */
    std::string getSummary() const override;

    /**
     * @brief Prints a readable invoice for the order.
     */
    void printInvoice() const;
};

#endif
