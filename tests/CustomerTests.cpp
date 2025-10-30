#include <gtest/gtest.h>
#include "include/Customer.h"
#include "include/DerivedCustomers.h"

// ============ RegularCustomer Tests ============

class RegularCustomerTest : public ::testing::Test {
protected:
    RegularCustomer* customer;
    
    void SetUp() override {
        customer = new RegularCustomer();
    }
    
    void TearDown() override {
        delete customer;
    }
};

TEST_F(RegularCustomerTest, InitialState) {
    EXPECT_EQ(customer->getCartSize(), 0);
    EXPECT_EQ(customer->getBudget(), 300.0);
    EXPECT_TRUE(customer->getCart().empty());
    EXPECT_EQ(customer->getCurrentRequest(), nullptr);
}

TEST_F(RegularCustomerTest, BudgetOperations) {
    EXPECT_TRUE(customer->canAfford(100.0));
    EXPECT_TRUE(customer->canAfford(300.0));
    EXPECT_FALSE(customer->canAfford(301.0));
    
    EXPECT_TRUE(customer->deductFromBudget(100.0));
    EXPECT_DOUBLE_EQ(customer->getBudget(), 200.0);
    
    EXPECT_FALSE(customer->deductFromBudget(300.0));
    EXPECT_DOUBLE_EQ(customer->getBudget(), 200.0);
}

TEST_F(RegularCustomerTest, SetBudget) {
    customer->setBudget(500.0);
    EXPECT_DOUBLE_EQ(customer->getBudget(), 500.0);
    
    customer->setBudget(-100.0);
    EXPECT_DOUBLE_EQ(customer->getBudget(), 500.0);
}

// TEST_F(RegularCustomerTest, NullSafety) {
//     EXPECT_NO_THROW(customer->addToCart(nullptr));
//     EXPECT_NO_THROW(customer->removeFromCart(nullptr));
//     EXPECT_NO_THROW(customer->clearCart());
//     EXPECT_NO_THROW(customer->checkOut());
// }

// ============ CorporateCustomer Tests ============

TEST(CorporateCustomerTest, HasHigherBudget) {
    CorporateCustomer customer;
    EXPECT_EQ(customer.getBudget(), 1000.0);
    EXPECT_EQ(customer.getCartSize(), 0);
}

// ============ WalkInCustomer Tests ============

TEST(WalkInCustomerTest, HasLowerBudget) {
    WalkInCustomer customer;
    EXPECT_EQ(customer.getBudget(), 150.0);
    EXPECT_EQ(customer.getCartSize(), 0);
}

// ============ Customer Type Comparison ============

TEST(CustomerTypes, BudgetDifferences) {
    WalkInCustomer walkIn;
    RegularCustomer regular;
    CorporateCustomer corporate;
    
    EXPECT_LT(walkIn.getBudget(), regular.getBudget());
    EXPECT_LT(regular.getBudget(), corporate.getBudget());
}