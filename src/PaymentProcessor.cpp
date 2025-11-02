

#include "include/PaymentProcessor.h"
#include <iostream>


void PaymentProcessor::processTransaction(FinalOrder* order) {
    std::cout << "\n[Transaction] Starting payment transaction...\n";

    if (!order) {
        std::cout << "[Error] Invalid order reference.\n";
        return;
    }

    verifyDetails();
    double amount = order->calculateTotalPrice();
    processPayment(amount);
    confirmTransaction();
    printReceipt(order);

    std::cout << "[Transaction] Payment completed successfully.\n";
}


void PaymentProcessor::handleFailure() {
    std::cout << "[Transaction] Payment failed. Please retry or use another method.\n";
}