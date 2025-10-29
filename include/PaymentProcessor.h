/**
 * @file PaymentProcessor.h
 * @author Gabi de Gouveia
 * @brief Abstract base class implementing the Template Method pattern for all payment types.
 */

#ifndef PAYMENTPROCESSOR_H
#define PAYMENTPROCESSOR_H

#include <iostream>
#include "FinalOrder.h"

/**
 * @class PaymentProcessor
 * @brief Defines the skeleton for processing payments using the Template Method pattern.
 *
 * The processTransaction() method defines the invariant sequence of payment steps:
 *  1. verifyDetails()
 *  2. processPayment(double)
 *  3. confirmTransaction()
 *  4. printReceipt(FinalOrder*)
 */
class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;

    /**
     * @brief Template Method that defines the steps for processing a payment.
     * @param order Pointer to the FinalOrder being processed.
     */
    void processTransaction(FinalOrder* order);

protected:
    /**
     * @brief Verifies the payment details (e.g. card info, cash).
     */
    virtual void verifyDetails() = 0;

    /**
     * @brief Executes the payment logic for the specific payment type.
     * @param amount The total payment amount.
     */
    virtual void processPayment(double amount) = 0;

    /**
     * @brief Confirms the payment transaction succeeded.
     */
    virtual void confirmTransaction() = 0;

    /**
     * @brief Prints the final receipt for the order.
     * @param order Pointer to the FinalOrder.
     */
    virtual void printReceipt(FinalOrder* order) = 0;

    /**
     * @brief Handles a failed transaction scenario.
     */
    virtual void handleFailure();
};

#endif
