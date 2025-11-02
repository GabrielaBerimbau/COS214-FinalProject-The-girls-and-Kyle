#include "FinalOrderScreen.h"
#include "ScreenManager.h"
#include "../include/Customer.h"
#include "../include/FinalOrder.h"
#include "../include/PaymentProcessor.h"
#include "../include/CashPayment.h"
#include "../include/CreditCardPayment.h"
#include "../include/Iterator.h"
#include <iostream>
#include <iomanip>
#include <sstream>

FinalOrderScreen::FinalOrderScreen(ScreenManager* mgr)
    : manager(mgr),
      currentState(CheckoutState::PAYMENT_SELECTION),
      finalOrder(nullptr),
      paymentProcessor(nullptr),
      orderTotal(0.0),
      customerBalance(0.0),
      paymentSuccessful(false),
      failureReason(""),
      cardNumber(""),
      cardExpiry(""),
      cardCVV(""),
      activeInputField(0),
      maxCardNumberLength(16),
      maxExpiryLength(5),
      maxCVVLength(3),
      receiptScrollOffset(0.0f),
      processingTimer(0.0f),
      cashHovered(false),
      cardHovered(false),
      confirmCardHovered(false),
      cancelCardHovered(false),
      continueHovered(false),
      retryHovered(false),
      exitToMenuHovered(false),
      exitProgramHovered(false) {

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    InitializeLayout();
    InitializeButtons();
}

FinalOrderScreen::~FinalOrderScreen() {
    if (paymentProcessor) {
        delete paymentProcessor;
        paymentProcessor = nullptr;
    }

}

void FinalOrderScreen::Reset() {
    currentState = CheckoutState::PAYMENT_SELECTION;
    paymentSuccessful = false;
    failureReason = "";
    cardNumber = "";
    cardExpiry = "";
    cardCVV = "";
    activeInputField = 0;
    receiptScrollOffset = 0.0f;
    processingTimer = 0.0f;
    receiptLines.clear();

    if (paymentProcessor) {
        delete paymentProcessor;
        paymentProcessor = nullptr;
    }

    InitializeOrderData();
}

void FinalOrderScreen::InitializeLayout() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
}

void FinalOrderScreen::InitializeButtons() {
    int buttonWidth = 250;
    int buttonHeight = 70;
    int buttonSpacing = 30;

    // Payment Selection buttons
    cashButton = Rectangle{
        static_cast<float>(screenWidth / 2 - buttonWidth - buttonSpacing / 2),
        static_cast<float>(screenHeight / 2 + 50),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    cardButton = Rectangle{
        static_cast<float>(screenWidth / 2 + buttonSpacing / 2),
        static_cast<float>(screenHeight / 2 + 50),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    int inputWidth = 400;
    int inputHeight = 50;
    int inputY = screenHeight / 2 - 100;
    int inputSpacing = 85; 

    cardNumberInput = Rectangle{
        static_cast<float>(screenWidth / 2 - inputWidth / 2),
        static_cast<float>(inputY),
        static_cast<float>(inputWidth),
        static_cast<float>(inputHeight)
    };

    cardExpiryInput = Rectangle{
        static_cast<float>(screenWidth / 2 - inputWidth / 2),
        static_cast<float>(inputY + inputSpacing),
        static_cast<float>(inputWidth / 2 - 10),
        static_cast<float>(inputHeight)
    };

    cardCVVInput = Rectangle{
        static_cast<float>(screenWidth / 2 + 10),
        static_cast<float>(inputY + inputSpacing),
        static_cast<float>(inputWidth / 2 - 10),
        static_cast<float>(inputHeight)
    };

    // Card Input buttons (autocomplete button not needed, using keyboard shortcut)
    int cardButtonWidth = 180;

    confirmCardButton = Rectangle{
        static_cast<float>(screenWidth / 2 - cardButtonWidth - 10),
        static_cast<float>(inputY + 200),
        static_cast<float>(cardButtonWidth),
        static_cast<float>(60)
    };

    cancelCardButton = Rectangle{
        static_cast<float>(screenWidth / 2 + 10),
        static_cast<float>(inputY + 200),
        static_cast<float>(cardButtonWidth),
        static_cast<float>(60)
    };

    // Success/Failure buttons
    continueButton = Rectangle{
        static_cast<float>(screenWidth / 2 - buttonWidth / 2),
        static_cast<float>(screenHeight / 2 + 80),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    retryButton = Rectangle{
        static_cast<float>(screenWidth / 2 - buttonWidth - 20),
        static_cast<float>(screenHeight / 2 + 80),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    exitToMenuButton = Rectangle{
        static_cast<float>(screenWidth / 2 + 20),
        static_cast<float>(screenHeight / 2 + 80),
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    // Receipt button
    exitProgramButton = Rectangle{
        static_cast<float>(screenWidth / 2 - buttonWidth / 2),
        static_cast<float>(screenHeight - 100),
        static_cast<float>(buttonWidth),
        static_cast<float>(60)
    };
}

void FinalOrderScreen::InitializeOrderData() {
    Customer* customer = manager->GetCustomer();
    if (!customer) {
        std::cout << "[FinalOrderScreen] Error: No customer found!" << std::endl;
        return;
    }

    // Get or create the final order
    finalOrder = manager->GetFinalOrder();
    if (!finalOrder) {
        finalOrder = customer->createFinalOrder();
        manager->SetFinalOrder(finalOrder);
    }

    // Get order total and customer balance
    orderTotal = finalOrder->calculateTotalPrice();
    customerBalance = customer->getBudget();

    std::cout << "[FinalOrderScreen] Order Total: R" << orderTotal << std::endl;
    std::cout << "[FinalOrderScreen] Customer Balance: R" << customerBalance << std::endl;
}

void FinalOrderScreen::GenerateReceiptLines() {
    receiptLines.clear();

    if (!finalOrder) {
        receiptLines.push_back("Error: No order found");
        return;
    }

    Customer* customer = manager->GetCustomer();
    std::string customerName = customer ? "Customer" : "Unknown";

    receiptLines.push_back("=======================================");
    receiptLines.push_back("          PLANT NURSERY SHOP          ");
    receiptLines.push_back("=======================================");
    receiptLines.push_back("");
    receiptLines.push_back("Invoice for: " + customerName);
    receiptLines.push_back("");

    receiptLines.push_back("ORDER ITEMS:");
    receiptLines.push_back("---------------------------------------");

    std::string formattedReceipt = finalOrder->getFormattedReceipt();

    std::istringstream receiptStream(formattedReceipt);
    std::string line;
    while (std::getline(receiptStream, line)) {
        if (!line.empty()) {
            receiptLines.push_back(line);
        }
    }

    receiptLines.push_back("---------------------------------------");

    std::ostringstream totalStream;
    totalStream << std::fixed << std::setprecision(2) << orderTotal;
    receiptLines.push_back("TOTAL: R" + totalStream.str());

    std::string paymentMethod = (paymentProcessor) ? "Card" : "Cash";
    if (dynamic_cast<CashPayment*>(paymentProcessor)) {
        paymentMethod = "Cash";
    } else if (dynamic_cast<CreditCardPayment*>(paymentProcessor)) {
        paymentMethod = "Credit Card";
    }
    receiptLines.push_back("Payment Method: " + paymentMethod);

    std::ostringstream balanceStream;
    balanceStream << std::fixed << std::setprecision(2) << customerBalance;
    receiptLines.push_back("Remaining Balance: R" + balanceStream.str());

    receiptLines.push_back("");
    receiptLines.push_back("=======================================");
    receiptLines.push_back("     Thank you for your purchase!     ");
    receiptLines.push_back("=======================================");
}

void FinalOrderScreen::Update() {
    switch (currentState) {
        case CheckoutState::PAYMENT_SELECTION:
            UpdatePaymentSelection();
            break;
        case CheckoutState::CARD_INPUT:
            UpdateCardInput();
            break;
        case CheckoutState::PROCESSING:
            UpdateProcessing();
            break;
        case CheckoutState::SUCCESS:
        case CheckoutState::FAILURE:
            UpdateSuccessFailure();
            break;
        case CheckoutState::RECEIPT:
            UpdateReceipt();
            break;
    }
}

void FinalOrderScreen::Draw() {
    ClearBackground(Color{216, 228, 220, 255}); // Soft sage green

    switch (currentState) {
        case CheckoutState::PAYMENT_SELECTION:
            DrawPaymentSelection();
            break;
        case CheckoutState::CARD_INPUT:
            DrawCardInput();
            break;
        case CheckoutState::PROCESSING:
            DrawProcessing();
            break;
        case CheckoutState::SUCCESS:
        case CheckoutState::FAILURE:
            DrawSuccessFailure();
            break;
        case CheckoutState::RECEIPT:
            DrawReceipt();
            break;
    }
}

// ==================== PAYMENT SELECTION ====================

void FinalOrderScreen::UpdatePaymentSelection() {
    Vector2 mousePos = GetMousePosition();

    cashHovered = CheckCollisionPointRec(mousePos, cashButton);
    cardHovered = CheckCollisionPointRec(mousePos, cardButton);

    if (cashHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        HandleCashPayment();
    }

    if (cardHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        HandleCardPayment();
    }
}

void FinalOrderScreen::DrawPaymentSelection() {
    DrawTextCentered("CHECKOUT", 80, 50, Color{85, 107, 95, 255}); // Dark forest green

    int boxWidth = 500;
    int boxHeight = 200;
    int boxX = screenWidth / 2 - boxWidth / 2;
    int boxY = 180;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{210, 210, 210, 255}); // Light grey
    DrawRectangleLinesEx(Rectangle{(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 3, Color{120, 120, 120, 255}); // Dark grey

    int textY = boxY + 30;
    int fontSize = 24;

    std::ostringstream totalStream;
    totalStream << std::fixed << std::setprecision(2) << orderTotal;
    std::string totalText = "Order Total: R" + totalStream.str();
    DrawTextCentered(totalText.c_str(), textY, fontSize, Color{180, 130, 50, 255}); // Dark gold/amber

    textY += 50;
    std::ostringstream balanceStream;
    balanceStream << std::fixed << std::setprecision(2) << customerBalance;
    std::string balanceText = "Your Balance: R" + balanceStream.str();
    DrawTextCentered(balanceText.c_str(), textY, fontSize, Color{40, 120, 60, 255}); // Dark green

    textY += 50;
    std::ostringstream remainingStream;
    remainingStream << std::fixed << std::setprecision(2) << (customerBalance - orderTotal);
    std::string remainingText = "After Purchase: R" + remainingStream.str();
    Color remainingColor = (customerBalance >= orderTotal) ? Color{85, 107, 95, 255} : Color{180, 40, 40, 255}; // Dark forest green : Dark red
    DrawTextCentered(remainingText.c_str(), textY, fontSize, remainingColor);

    DrawTextCentered("Select Payment Method:", screenHeight / 2 - 20, 28, Color{85, 107, 95, 255}); // Dark forest green

    Color cashColor = cashHovered ? Color{50, 130, 80, 255} : Color{60, 150, 100, 255}; // Lighter green shades
    DrawRectangleRec(cashButton, cashColor);
    DrawRectangleLinesEx(cashButton, 3, WHITE);
    const char* cashText = "CASH";
    int cashTextWidth = MeasureText(cashText, 30);
    DrawText(cashText,
             cashButton.x + (cashButton.width - cashTextWidth) / 2,
             cashButton.y + (cashButton.height - 30) / 2,
             30,
             WHITE);

    Color cardColor = cardHovered ? Color{60, 90, 140, 255} : Color{70, 100, 150, 255}; // Lighter blue shades
    DrawRectangleRec(cardButton, cardColor);
    DrawRectangleLinesEx(cardButton, 3, WHITE);
    const char* cardText = "CREDIT CARD";
    int cardTextWidth = MeasureText(cardText, 30);
    DrawText(cardText,
             cardButton.x + (cardButton.width - cardTextWidth) / 2,
             cardButton.y + (cardButton.height - 30) / 2,
             30,
             WHITE);

    if (customerBalance < orderTotal) {
        DrawTextCentered("WARNING: Insufficient funds!", screenHeight - 80, 22, RED);
    }
}

void FinalOrderScreen::HandleCashPayment() {
    std::cout << "[FinalOrderScreen] Cash payment selected" << std::endl;
    currentState = CheckoutState::PROCESSING;
    processingTimer = 0.0f;

    //Process payment immediately for cash
    bool success = ProcessPayment(true);
}

void FinalOrderScreen::HandleCardPayment() {
    std::cout << "[FinalOrderScreen] Card payment selected, showing input form" << std::endl;
    currentState = CheckoutState::CARD_INPUT;
}

// ==================== CARD INPUT ====================

void FinalOrderScreen::UpdateCardInput() {
    Vector2 mousePos = GetMousePosition();

    confirmCardHovered = CheckCollisionPointRec(mousePos, confirmCardButton);
    cancelCardHovered = CheckCollisionPointRec(mousePos, cancelCardButton);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePos, cardNumberInput)) {
            activeInputField = 1;
        } else if (CheckCollisionPointRec(mousePos, cardExpiryInput)) {
            activeInputField = 2;
        } else if (CheckCollisionPointRec(mousePos, cardCVVInput)) {
            activeInputField = 3;
        } else {
            activeInputField = 0;
        }
    }

    int key = GetCharPressed();
    while (key > 0) {
        if (activeInputField == 1 && cardNumber.length() < maxCardNumberLength) {
            if ((key >= '0' && key <= '9')) {
                cardNumber += (char)key;
            }
        } else if (activeInputField == 2 && cardExpiry.length() < maxExpiryLength) {
            if ((key >= '0' && key <= '9') || key == '/') {
                cardExpiry += (char)key;
            }
        } else if (activeInputField == 3 && cardCVV.length() < maxCVVLength) {
            if ((key >= '0' && key <= '9')) {
                cardCVV += (char)key;
            }
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (activeInputField == 1 && !cardNumber.empty()) {
            cardNumber.pop_back();
        } else if (activeInputField == 2 && !cardExpiry.empty()) {
            cardExpiry.pop_back();
        } else if (activeInputField == 3 && !cardCVV.empty()) {
            cardCVV.pop_back();
        }
    }

    if (IsKeyPressed(KEY_A)) {
        HandleAutocomplete();
    }

    if (confirmCardHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        HandleConfirmCard();
    }

    if (cancelCardHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        HandleCancelCard();
    }
}

void FinalOrderScreen::DrawCardInput() {
    DrawTextCentered("Enter Card Details", 60, 40, WHITE);

    DrawText("Card Number:", cardNumberInput.x, cardNumberInput.y - 25, 18, Color{120, 140, 125, 255}); // Medium sage
    Color numberBorder = (activeInputField == 1) ? Color{235, 186, 170, 255} : Color{200, 210, 205, 255}; // Warm terracotta : Light sage
    DrawRectangleRec(cardNumberInput, Color{245, 250, 247, 255}); // Very light sage
    DrawRectangleLinesEx(cardNumberInput, 2, numberBorder);
    DrawText(cardNumber.c_str(), cardNumberInput.x + 10, cardNumberInput.y + 15, 20, Color{85, 107, 95, 255}); // Dark forest green

    DrawText("Expiry (MM/YY):", cardExpiryInput.x, cardExpiryInput.y - 25, 18, Color{120, 140, 125, 255}); // Medium sage
    Color expiryBorder = (activeInputField == 2) ? Color{235, 186, 170, 255} : Color{200, 210, 205, 255}; // Warm terracotta : Light sage
    DrawRectangleRec(cardExpiryInput, Color{245, 250, 247, 255}); // Very light sage
    DrawRectangleLinesEx(cardExpiryInput, 2, expiryBorder);
    DrawText(cardExpiry.c_str(), cardExpiryInput.x + 10, cardExpiryInput.y + 15, 20, Color{85, 107, 95, 255}); // Dark forest green

    DrawText("CVV:", cardCVVInput.x, cardCVVInput.y - 25, 18, Color{120, 140, 125, 255}); // Medium sage
    Color cvvBorder = (activeInputField == 3) ? Color{235, 186, 170, 255} : Color{200, 210, 205, 255}; // Warm terracotta : Light sage
    DrawRectangleRec(cardCVVInput, Color{245, 250, 247, 255}); // Very light sage
    DrawRectangleLinesEx(cardCVVInput, 2, cvvBorder);

    std::string maskedCVV(cardCVV.length(), '*');
    DrawText(maskedCVV.c_str(), cardCVVInput.x + 10, cardCVVInput.y + 15, 20, Color{85, 107, 95, 255}); // Dark forest green

    const char* hintText = "Press 'A' to autocomplete (demo)";
    DrawText(hintText, screenWidth - 280, screenHeight - 100, 16, Color{150, 150, 150, 255});

    // Confirm button
    Color confirmColor = confirmCardHovered ? Color{50, 130, 80, 255} : Color{60, 150, 100, 255}; // Lighter green shades
    DrawRectangleRec(confirmCardButton, confirmColor);
    DrawRectangleLinesEx(confirmCardButton, 2, WHITE);
    const char* confirmText = "Confirm Payment";
    int confirmTextWidth = MeasureText(confirmText, 22);
    DrawText(confirmText,
             confirmCardButton.x + (confirmCardButton.width - confirmTextWidth) / 2,
             confirmCardButton.y + 18,
             22,
             WHITE);

    // Cancel button
    Color cancelColor = cancelCardHovered ? MAROON : Color{120, 50, 50, 255};
    DrawRectangleRec(cancelCardButton, cancelColor);
    DrawRectangleLinesEx(cancelCardButton, 2, WHITE);
    const char* cancelText = "Cancel";
    int cancelTextWidth = MeasureText(cancelText, 22);
    DrawText(cancelText,
             cancelCardButton.x + (cancelCardButton.width - cancelTextWidth) / 2,
             cancelCardButton.y + 18,
             22,
             WHITE);
}

void FinalOrderScreen::HandleAutocomplete() {
    std::cout << "[FinalOrderScreen] Autocompleting card details for demo" << std::endl;
    cardNumber = "4532015112830366";
    cardExpiry = "12/25";
    cardCVV = "123";
}

void FinalOrderScreen::HandleConfirmCard() {
    if (!ValidateCardInput()) {
        std::cout << "[FinalOrderScreen] Invalid card details" << std::endl;
        return;
    }

    std::cout << "[FinalOrderScreen] Card details confirmed, processing payment" << std::endl;
    currentState = CheckoutState::PROCESSING;
    processingTimer = 0.0f;

    ProcessPayment(false);
}

void FinalOrderScreen::HandleCancelCard() {
    std::cout << "[FinalOrderScreen] Card input cancelled" << std::endl;
    cardNumber = "";
    cardExpiry = "";
    cardCVV = "";
    activeInputField = 0;
    currentState = CheckoutState::PAYMENT_SELECTION;
}

bool FinalOrderScreen::ValidateCardInput() {
    if (cardNumber.length() < 13 || cardNumber.length() > 16) {
        failureReason = "Invalid card number length";
        return false;
    }

    if (cardExpiry.length() != 5 || cardExpiry[2] != '/') {
        failureReason = "Invalid expiry format (use MM/YY)";
        return false;
    }

    if (cardCVV.length() != 3) {
        failureReason = "Invalid CVV (must be 3 digits)";
        return false;
    }

    return true;
}

// ==================== PROCESSING ====================

void FinalOrderScreen::UpdateProcessing() {
    processingTimer += GetFrameTime();

    // After 2 seconds, transition to success/failure
    if (processingTimer >= 2.0f) {
        currentState = paymentSuccessful ? CheckoutState::SUCCESS : CheckoutState::FAILURE;
    }
}

void FinalOrderScreen::DrawProcessing() {
    DrawTextCentered("Processing Payment...", screenHeight / 2 - 50, 40, YELLOW);

    // Animated dots
    int dotCount = ((int)(processingTimer * 2.0f)) % 4;
    std::string dots(dotCount, '.');
    DrawTextCentered(dots.c_str(), screenHeight / 2 + 20, 30, WHITE);

    // Spinner
    float angle = processingTimer * 360.0f;
    Vector2 center = {(float)screenWidth / 2, (float)screenHeight / 2 + 80};
    DrawRing(center, 20, 25, 0, angle, 36, SKYBLUE);
}

// ==================== SUCCESS/FAILURE ====================

void FinalOrderScreen::UpdateSuccessFailure() {
    Vector2 mousePos = GetMousePosition();

    if (currentState == CheckoutState::SUCCESS) {
        continueHovered = CheckCollisionPointRec(mousePos, continueButton);

        if (continueHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            HandleContinue();
        }
    } else {
        retryHovered = CheckCollisionPointRec(mousePos, retryButton);
        exitToMenuHovered = CheckCollisionPointRec(mousePos, exitToMenuButton);

        if (retryHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            HandleRetry();
        }

        if (exitToMenuHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            HandleExitToMenu();
        }
    }
}

void FinalOrderScreen::DrawSuccessFailure() {
    if (currentState == CheckoutState::SUCCESS) {
        // Success screen
        DrawTextCentered("PAYMENT SUCCESSFUL!", screenHeight / 2 - 120, 45, LIME);

        DrawTextCentered("Your payment has been processed.", screenHeight / 2 - 40, 24, WHITE);
        DrawTextCentered("Thank you for your purchase!", screenHeight / 2 - 5, 24, WHITE);

        // Continue button
        Color continueColor = continueHovered ? Color{50, 130, 80, 255} : Color{60, 150, 100, 255}; // Lighter green shades
        DrawRectangleRec(continueButton, continueColor);
        DrawRectangleLinesEx(continueButton, 3, WHITE);
        const char* continueText = "View Receipt";
        int continueTextWidth = MeasureText(continueText, 28);
        DrawText(continueText,
                 continueButton.x + (continueButton.width - continueTextWidth) / 2,
                 continueButton.y + 20,
                 28,
                 WHITE);
    } else {
        // Failure screen
        DrawTextCentered("PAYMENT FAILED", screenHeight / 2 - 120, 45, RED);

        std::string reason = failureReason.empty() ? "Insufficient funds" : failureReason;
        DrawTextCentered(reason.c_str(), screenHeight / 2 - 50, 24, ORANGE);

        DrawTextCentered("Would you like to try again?", screenHeight / 2 + 10, 22, LIGHTGRAY);

        // Retry button
        Color retryColor = retryHovered ? Color{50, 130, 80, 255} : Color{60, 150, 100, 255}; // Lighter green shades
        DrawRectangleRec(retryButton, retryColor);
        DrawRectangleLinesEx(retryButton, 3, WHITE);
        const char* retryText = "Try Again";
        int retryTextWidth = MeasureText(retryText, 26);
        DrawText(retryText,
                 retryButton.x + (retryButton.width - retryTextWidth) / 2,
                 retryButton.y + 22,
                 26,
                 WHITE);

        // Exit button
        Color exitColor = exitToMenuHovered ? MAROON : Color{120, 50, 50, 255};
        DrawRectangleRec(exitToMenuButton, exitColor);
        DrawRectangleLinesEx(exitToMenuButton, 3, WHITE);
        const char* exitText = "Exit to Menu";
        int exitTextWidth = MeasureText(exitText, 26);
        DrawText(exitText,
                 exitToMenuButton.x + (exitToMenuButton.width - exitTextWidth) / 2,
                 exitToMenuButton.y + 22,
                 26,
                 WHITE);
    }
}

void FinalOrderScreen::HandleContinue() {
    std::cout << "[FinalOrderScreen] Continuing to receipt" << std::endl;
    GenerateReceiptLines();
    currentState = CheckoutState::RECEIPT;
}

void FinalOrderScreen::HandleRetry() {
    std::cout << "[FinalOrderScreen] Retrying payment" << std::endl;
    cardNumber = "";
    cardExpiry = "";
    cardCVV = "";
    activeInputField = 0;
    failureReason = "";
    currentState = CheckoutState::PAYMENT_SELECTION;
}

void FinalOrderScreen::HandleExitToMenu() {
    std::cout << "[FinalOrderScreen] Exiting to main menu" << std::endl;
    manager->SwitchScreen(GameScreen::START);
}

// ==================== RECEIPT ====================

void FinalOrderScreen::UpdateReceipt() {
    Vector2 mousePos = GetMousePosition();
    exitProgramHovered = CheckCollisionPointRec(mousePos, exitProgramButton);

    // Handle scrolling
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        receiptScrollOffset -= wheelMove * 30.0f;

        // Clamp scroll
        float maxScroll = receiptLines.size() * 25.0f - 400.0f;
        if (receiptScrollOffset < 0) receiptScrollOffset = 0;
        if (receiptScrollOffset > maxScroll && maxScroll > 0) receiptScrollOffset = maxScroll;
    }

    if (exitProgramHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        HandleExitProgram();
    }
}

void FinalOrderScreen::DrawReceipt() {
    DrawTextCentered("RECEIPT", 50, 40, LIME);

    // Receipt box
    int boxWidth = 600;
    int boxHeight = 500;
    int boxX = screenWidth / 2 - boxWidth / 2;
    int boxY = 120;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{250, 250, 240, 255});
    DrawRectangleLinesEx(Rectangle{(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 3, BLACK);

    // Enable scissor mode for scrolling
    BeginScissorMode(boxX, boxY, boxWidth, boxHeight);

    int textY = boxY + 20 - (int)receiptScrollOffset;
    for (const std::string& line : receiptLines) {
        DrawText(line.c_str(), boxX + 20, textY, 18, BLACK);
        textY += 25;
    }

    EndScissorMode();

    // Scroll hint
    if (receiptLines.size() * 25 > boxHeight) {
        DrawText("Scroll with mouse wheel", boxX + boxWidth / 2 - 100, boxY + boxHeight + 10, 16, LIGHTGRAY);
    }

    // Exit button
    Color exitColor = exitProgramHovered ? DARKGREEN : Color{40, 100, 60, 255};
    DrawRectangleRec(exitProgramButton, exitColor);
    DrawRectangleLinesEx(exitProgramButton, 3, WHITE);
    const char* exitText = "Return to Main Menu";
    int exitTextWidth = MeasureText(exitText, 24);
    DrawText(exitText,
             exitProgramButton.x + (exitProgramButton.width - exitTextWidth) / 2,
             exitProgramButton.y + 17,
             24,
             WHITE);
}

void FinalOrderScreen::HandleExitProgram() {
    std::cout << "[FinalOrderScreen] Exiting program" << std::endl;
    manager->SwitchScreen(GameScreen::START);
}

// ==================== PAYMENT PROCESSING ====================

bool FinalOrderScreen::ProcessPayment(bool isCash) {
    Customer* customer = manager->GetCustomer();
    if (!customer || !finalOrder) {
        paymentSuccessful = false;
        failureReason = "System error: Invalid customer or order";
        return false;
    }

    // Check if customer can afford
    if (!customer->canAfford(orderTotal)) {
        std::cout << "[FinalOrderScreen] Payment failed: Insufficient funds" << std::endl;
        paymentSuccessful = false;
        failureReason = "Insufficient funds";
        return false;
    }

    // Create appropriate payment processor
    if (paymentProcessor) {
        delete paymentProcessor;
    }

    if (isCash) {
        paymentProcessor = new CashPayment();
    } else {
        paymentProcessor = new CreditCardPayment();
    }

    // Process transaction using template method
    std::cout << "[FinalOrderScreen] Processing transaction..." << std::endl;
    paymentProcessor->processTransaction(finalOrder);

    // Deduct from customer budget
    bool deducted = customer->deductFromBudget(orderTotal);

    if (deducted) {
        paymentSuccessful = true;
        customerBalance = customer->getBudget();

        // Cleanup will be done when viewing receipt
        std::cout << "[FinalOrderScreen] Payment successful! New balance: R" << customerBalance << std::endl;
        return true;
    } else {
        paymentSuccessful = false;
        failureReason = "Failed to deduct from budget";
        return false;
    }
}

void FinalOrderScreen::CleanupAfterPayment() {
    Customer* customer = manager->GetCustomer();
    if (!customer) return;

    std::cout << "[FinalOrderScreen] Cleaning up after successful payment" << std::endl;

}


void FinalOrderScreen::DrawTextCentered(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (screenWidth - textWidth) / 2, y, fontSize, color);
}

void FinalOrderScreen::DrawInfoBox(const char* label, const char* value, int x, int y, int width) {
    int height = 50;
    DrawRectangle(x, y, width, height, Color{245, 250, 247, 255}); // Very light sage
    DrawRectangleLinesEx(Rectangle{(float)x, (float)y, (float)width, (float)height}, 2, Color{200, 210, 205, 255}); // Light sage

    DrawText(label, x + 10, y + 10, 18, Color{120, 140, 125, 255}); // Medium sage
    DrawText(value, x + 10, y + 28, 20, Color{85, 107, 95, 255}); // Dark forest green
}
