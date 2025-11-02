
#include "include/CashPayment.h"
#include <iostream>


void CashPayment::verifyDetails() {
    std::cout << "[CashPayment] Verifying available cash...\n";
}

void CashPayment::processPayment(double amount) {
    std::cout << "[CashPayment] Customer hands over R" << amount << " in cash.\n";
    std::cout << "[CashPayment] Counting and verifying cash.\n";
}

void CashPayment::confirmTransaction() {
    std::cout << "[CashPayment] Cash transaction confirmed.\n";
}

void CashPayment::printReceipt(FinalOrder* order) {
    std::cout << "[Receipt] Printing cash payment receipt:\n";
    order->printInvoice();
}
