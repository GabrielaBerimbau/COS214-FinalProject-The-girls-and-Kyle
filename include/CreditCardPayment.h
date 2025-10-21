#ifndef CREDITCARDPAYMENT_H
#define CREDITCARDPAYMENT_H
#include "PaymentProcessor.h"

class CreditCardPayment : public PaymentProcessor {
public:
    void processPayment() override;
};

#endif
