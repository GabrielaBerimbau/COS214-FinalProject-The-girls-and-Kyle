#ifndef FINAL_ORDER_SCREEN_H
#define FINAL_ORDER_SCREEN_H

#include "raylib.h"
#include <string>
#include <vector>

class ScreenManager;
class FinalOrder;
class PaymentProcessor;

/**
 * @brief Screen states for the final order checkout process
 */
enum class CheckoutState {
    PAYMENT_SELECTION,    // Choose Cash or Card
    CARD_INPUT,          // Enter card details (if Card selected)
    PROCESSING,          // Processing payment animation
    SUCCESS,             // Payment successful
    FAILURE,             // Payment failed
    RECEIPT              // Display receipt
};

/**
 * @brief Screen for handling final order checkout and payment processing
 *
 * This screen manages the complete checkout flow:
 * - Display order total and customer balance
 * - Payment method selection (Cash/Card)
 * - Card details input with validation
 * - Payment processing using Template Method pattern
 * - Success/Failure handling
 * - Receipt display
 */
class FinalOrderScreen {
private:
    ScreenManager* manager;

    // Screen dimensions
    int screenWidth;
    int screenHeight;

    // Current state
    CheckoutState currentState;

    // Order and payment data
    FinalOrder* finalOrder;
    PaymentProcessor* paymentProcessor;
    double orderTotal;
    double customerBalance;
    bool paymentSuccessful;
    std::string failureReason;

    // Card input fields
    std::string cardNumber;
    std::string cardExpiry;
    std::string cardCVV;
    int activeInputField; // 0=none, 1=number, 2=expiry, 3=cvv
    int maxCardNumberLength;
    int maxExpiryLength;
    int maxCVVLength;

    // Receipt data
    std::vector<std::string> receiptLines;
    float receiptScrollOffset;

    // UI Buttons - Payment Selection
    Rectangle cashButton;
    Rectangle cardButton;

    // UI Buttons - Card Input
    Rectangle cardNumberInput;
    Rectangle cardExpiryInput;
    Rectangle cardCVVInput;
    Rectangle confirmCardButton;
    Rectangle cancelCardButton;

    // UI Buttons - Processing
    float processingTimer;

    // UI Buttons - Success/Failure
    Rectangle continueButton;
    Rectangle retryButton;
    Rectangle exitToMenuButton;

    // UI Buttons - Receipt
    Rectangle exitProgramButton;

    // Hover states
    bool cashHovered;
    bool cardHovered;
    bool confirmCardHovered;
    bool cancelCardHovered;
    bool continueHovered;
    bool retryHovered;
    bool exitToMenuHovered;
    bool exitProgramHovered;

    // Helper methods - Initialization
    void InitializeLayout();
    void InitializeButtons();
    void InitializeOrderData();
    void GenerateReceiptLines();

    // Helper methods - Update
    void UpdatePaymentSelection();
    void UpdateCardInput();
    void UpdateProcessing();
    void UpdateSuccessFailure();
    void UpdateReceipt();

    // Helper methods - Draw
    void DrawPaymentSelection();
    void DrawCardInput();
    void DrawProcessing();
    void DrawSuccessFailure();
    void DrawReceipt();

    // Helper methods - Actions
    void HandleCashPayment();
    void HandleCardPayment();
    void HandleAutocomplete();
    void HandleConfirmCard();
    void HandleCancelCard();
    void HandleRetry();
    void HandleExitToMenu();
    void HandleContinue();
    void HandleExitProgram();

    // Helper methods - Payment Processing
    bool ProcessPayment(bool isCash);
    void CleanupAfterPayment();
    bool ValidateCardInput();

    // Helper methods - Drawing utilities
    void DrawInfoBox(const char* label, const char* value, int x, int y, int width);
    void DrawTextCentered(const char* text, int y, int fontSize, Color color);

public:
    FinalOrderScreen(ScreenManager* mgr);
    ~FinalOrderScreen();

    void Update();
    void Draw();
    void Reset(); // Reset when entering screen
};

#endif // FINAL_ORDER_SCREEN_H
