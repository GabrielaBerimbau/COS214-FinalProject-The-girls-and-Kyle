/**
 * @file CreditCardPayment.h
 * @author Gabi de Gouveia
 * @brief Defines a concrete payment processor for credit card transactions.
 */
#ifndef CREDITCARDPAYMENT_H
#define CREDITCARDPAYMENT_H

#include "PaymentProcessor.h"
#include <iostream>

/**
 * @class CreditCardPayment
 * @brief Implements credit card payment steps for the Template Method pattern.
 *
 * Handles verification, processing, confirmation, and receipt printing
 * specific to credit card transactions.
 */
class CreditCardPayment : public PaymentProcessor {
protected:

    /**
     * @brief Validates the customer's credit card details.
     */
    void verifyDetails() override;

    /**
     * @brief Processes a credit card charge for the specified amount.
     * @param amount The total amount to charge.
     */
    void processPayment(double amount) override;

    /**
     * @brief Confirms that the credit card transaction succeeded.
     */
    void confirmTransaction() override;

    /**
     * @brief Prints a receipt for a credit card transaction.
     * @param order Pointer to the FinalOrder being processed.
     */
    void printReceipt(FinalOrder* order) override;
};

#endif
