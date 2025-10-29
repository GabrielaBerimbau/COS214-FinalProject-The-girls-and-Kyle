
#include <gtest/gtest.h>
#include "include/PaymentProcessor.h"
#include "include/CashPayment.h"
#include "include/CreditCardPayment.h"
#include "include/FinalOrder.h"

class TemplateMethodTest : public ::testing::Test {
protected:
    FinalOrder* order;
    CashPayment* cashPayment;
    CreditCardPayment* cardPayment;

    void SetUp() override {
        order = new FinalOrder("John Doe");
        cashPayment = new CashPayment();
        cardPayment = new CreditCardPayment();
    }

    void TearDown() override {
        delete cashPayment;
        delete cardPayment;
        delete order;
    }
};

TEST_F(TemplateMethodTest, CashPaymentExecution) {
    testing::internal::CaptureStdout();
    cashPayment->processTransaction(order);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[CashPayment] Customer hands over"), std::string::npos)
        << "Cash handover message missing";
    EXPECT_NE(output.find("[CashPayment] Cash"), std::string::npos)
        << "Cash verification message missing";
    EXPECT_NE(output.find("[Transaction] Payment completed successfully."), std::string::npos)
        << "Transaction completion message missing";
}

TEST_F(TemplateMethodTest, CreditCardPaymentExecution) {
    testing::internal::CaptureStdout();
    cardPayment->processTransaction(order);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[CreditCardPayment]"), std::string::npos)
        << "Credit card process message missing";
    EXPECT_NE(output.find("credit"), std::string::npos)
        << "Credit card flow message missing";
    EXPECT_NE(output.find("[Transaction] Payment completed successfully."), std::string::npos)
        << "Transaction completion message missing";
}

TEST_F(TemplateMethodTest, NullOrderSafety) {
    testing::internal::CaptureStdout();
    cashPayment->processTransaction(nullptr);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Error] Invalid order reference."), std::string::npos);
}
