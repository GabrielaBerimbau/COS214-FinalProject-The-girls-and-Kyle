#ifndef CASHPAYMENT_H
#define CASHPAYMENT_H
#include "PaymentProcessor.h"

class CashPayment : public PaymentProcessor {
public:
    void processPayment() override;
};

#endif
