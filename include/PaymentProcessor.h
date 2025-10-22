#ifndef PAYMENTPROCESSOR_H
#define PAYMENTPROCESSOR_H
#include <iostream>
#include <string>

class Plant;

class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;
    void processTransaction();
    virtual void processPayment() = 0;
    bool payForPlant(Plant* plant);

protected:
    virtual bool verifyDetails();
    virtual void confirmTransaction();
    virtual void printReceipt();
    virtual void handleFailure();
};

#endif
