

#include "include/CreditCardPayment.h"
#include <iostream>

void CreditCardPayment::verifyDetails() {
    std::cout << "[CreditCardPayment] Verifying card details (number, expiry, CVV)...\n";
}

void CreditCardPayment::processPayment(double amount) {
    std::cout << "[CreditCardPayment] Charging R" << amount << " to the customer's credit card...\n";
    std::cout << "[CreditCardPayment] Waiting for payment gateway approval...\n";
}

void CreditCardPayment::confirmTransaction() {
    std::cout << "[CreditCardPayment] Transaction approved and confirmed.\n";
}

void CreditCardPayment::printReceipt(FinalOrder* order) {
    std::cout << "[Receipt] Printing credit card payment receipt:\n";
    order->printInvoice();
}
