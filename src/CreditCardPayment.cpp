/**
 * @file CreditCardPayment.cpp
 * @brief Implements credit card payment processing logic.
 */

#include "include/CreditCardPayment.h"
#include <iostream>

/**
 * @brief Validates the customer's credit card details.
 */
void CreditCardPayment::verifyDetails() {
    std::cout << "[CreditCardPayment] Verifying card details (number, expiry, CVV)...\n";
}
/**
 * @brief Processes a credit card charge for the specified amount.
 * @param amount The total amount to charge.
 */
void CreditCardPayment::processPayment(double amount) {
    std::cout << "[CreditCardPayment] Charging R" << amount << " to the customer's credit card...\n";
    std::cout << "[CreditCardPayment] Waiting for payment gateway approval...\n";
}
/**
 * @brief Confirms that the credit card transaction succeeded.
 */
void CreditCardPayment::confirmTransaction() {
    std::cout << "[CreditCardPayment] Transaction approved and confirmed.\n";
}
/**
 * @brief Prints a receipt for a credit card transaction.
 * @param order Pointer to the FinalOrder being processed.
 */
void CreditCardPayment::printReceipt(FinalOrder* order) {
    std::cout << "[Receipt] Printing credit card payment receipt:\n";
    order->printInvoice();
}
