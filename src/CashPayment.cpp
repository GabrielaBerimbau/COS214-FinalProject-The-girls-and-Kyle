/**
 * @file CashPayment.cpp
 * @author Gabi de Gouveia
 * @brief Implements cash payment using the Template Method pattern.
 */
#include "include/CashPayment.h"
#include <iostream>

/**
 * @brief Verifies if the customer has sufficient cash for payment.
 */
void CashPayment::verifyDetails() {
    std::cout << "[CashPayment] Verifying available cash...\n";
}
/**
 * @brief Handles the actual cash payment process.
 * @param amount The total amount due.
 */
void CashPayment::processPayment(double amount) {
    std::cout << "[CashPayment] Customer hands over R" << amount << " in cash.\n";
    std::cout << "[CashPayment] Counting and verifying cash.\n";
}
/**
 * @brief Confirms that the cash payment has been accepted successfully.
 */
void CashPayment::confirmTransaction() {
    std::cout << "[CashPayment] Cash transaction confirmed.\n";
}
/**
 * @brief Prints a receipt for a cash payment.
 * @param order Pointer to the FinalOrder being paid for.
 */
void CashPayment::printReceipt(FinalOrder* order) {
    std::cout << "[Receipt] Printing cash payment receipt:\n";
    order->printInvoice();
}
