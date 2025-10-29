/**
 * @file CashPayment.h
 * @author Gabi de Gouveia
 * @brief Defines a concrete payment processor for cash transactions.
 */
#ifndef CASHPAYMENT_H
#define CASHPAYMENT_H

#include "PaymentProcessor.h"
#include <iostream>

/**
 * @class CashPayment
 * @brief Implements cash payment steps for the Template Method pattern.
 *
 * Handles verification, processing, confirmation, and receipt printing
 * specific to cash transactions.
 */
class CashPayment : public PaymentProcessor {
protected:
    /**
     * @brief Verifies if the customer has sufficient cash for payment.
     */
    void verifyDetails() override;

    /**
     * @brief Handles the actual cash payment process.
     * @param amount The total amount due.
     */
    void processPayment(double amount) override;

    /**
     * @brief Confirms that the cash payment has been accepted successfully.
     */
    void confirmTransaction() override;

    /**
     * @brief Prints a receipt for a cash payment.
     * @param order Pointer to the FinalOrder being paid for.
     */
    void printReceipt(FinalOrder* order) override;
};

#endif
