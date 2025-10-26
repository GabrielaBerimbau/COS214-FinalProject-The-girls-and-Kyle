/**
 * @file CashPayment.h
 * @brief Defines a concrete payment processor for cash transactions.
 */
#ifndef CASHPAYMENT_H
#define CASHPAYMENT_H

#include "PaymentProcessor.h"
#include <iostream>

/**
 * @class CashPayment
 * @brief Implements cash payment steps for the Template Method pattern.
 */
class CashPayment : public PaymentProcessor {
protected:
    void verifyDetails() override;
    void processPayment(double amount) override;
    void confirmTransaction() override;
    void printReceipt(FinalOrder* order) override;
};

#endif
