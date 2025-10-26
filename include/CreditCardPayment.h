/**
 * @file CreditCardPayment.h
 * @brief Defines a concrete payment processor for credit card transactions.
 */
#ifndef CREDITCARDPAYMENT_H
#define CREDITCARDPAYMENT_H

#include "PaymentProcessor.h"
#include <iostream>

/**
 * @class CreditCardPayment
 * @brief Implements credit card payment steps for the Template Method pattern.
 */
class CreditCardPayment : public PaymentProcessor {
protected:
    void verifyDetails() override;
    void processPayment(double amount) override;
    void confirmTransaction() override;
    void printReceipt(FinalOrder* order) override;
};

#endif
