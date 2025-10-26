/**
 * @file PaymentProcessor.cpp
 * @brief Implements the Template Method algorithm for payment processing.
 */

#include "include/PaymentProcessor.h"
#include <iostream>

/**
 * @brief Executes the payment process for a given FinalOrder.
 * This method calls the predefined sequence of steps implemented by subclasses.
 * @param order Pointer to the FinalOrder being paid for.
 */
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

/**
 * @brief Handles failures in the payment process.
 */
void PaymentProcessor::handleFailure() {
    std::cout << "[Transaction] Payment failed. Please retry or use another method.\n";
}