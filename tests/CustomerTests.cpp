#include <gtest/gtest.h>
#include "include/Customer.h"
#include "include/DerivedCustomers.h"
#include "include/Plant.h"
#include "include/NurseryMediator.h"
#include "include/NurseryCoordinator.h"
#include "include/SalesFloor.h"
#include "include/Greenhouse.h"
#include "include/ConcreteOrder.h"
#include "include/FinalOrder.h"
#include "include/Leaf.h"
#include "include/Request.h"
#include "include/StaffMembers.h"
#include "include/SalesAssistant.h"
#include "include/FlowerCareStrategy.h"
#include "include/MatureState.h"
#include "include/RibbonDecorator.h"
#include "include/GiftWrapDecorator.h"
#include "include/DecorativePotDecorator.h"

// ============ Test Fixture for Customer Tests ============

class CustomerTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    SalesFloor* salesFloor;
    Greenhouse* greenhouse;
    RegularCustomer* customer;
    Plant* testPlant1;
    Plant* testPlant2;
    Plant* testPlant3;
    
    void SetUp() override {
        // Create infrastructure
        mediator = new NurseryMediator();
        salesFloor = new SalesFloor(mediator, 3, 3);
        greenhouse = new Greenhouse(mediator, 2, 2);
        
        mediator->registerColleague(salesFloor);
        mediator->registerColleague(greenhouse);
        
        // Create customer
        customer = new RegularCustomer();
        customer->setMediator(mediator);
        customer->setName("Test Customer");
        customer->setId("TEST-001");
        customer->setBudget(500.0);
        mediator->registerColleague(customer);
        
        // Create test plants
        testPlant1 = new Plant("Rose", "R001", new FlowerCareStrategy(), new MatureState());
        testPlant1->setPrice(50.0);
        testPlant1->setReadyForSale(true);
        
        testPlant2 = new Plant("Daisy", "D001", new FlowerCareStrategy(), new MatureState());
        testPlant2->setPrice(30.0);
        testPlant2->setReadyForSale(true);
        
        testPlant3 = new Plant("Tulip", "T001", new FlowerCareStrategy(), new MatureState());
        testPlant3->setPrice(40.0);
        testPlant3->setReadyForSale(true);
        
        // Place plants on sales floor
        salesFloor->addPlantToDisplay(testPlant1, 0, 0);
        salesFloor->addPlantToDisplay(testPlant2, 0, 1);
        salesFloor->addPlantToDisplay(testPlant3, 1, 0);
    }
    
    void TearDown() override {
        delete customer;
        delete salesFloor;
        delete greenhouse;
        delete mediator;
    }
};

// ============ Basic Customer Properties Tests ============

TEST_F(CustomerTest, InitialState) {
    EXPECT_EQ(customer->getCartSize(), 0);
    EXPECT_EQ(customer->getBudget(), 500.0);
    EXPECT_TRUE(customer->getCart().empty());
    EXPECT_EQ(customer->getCurrentRequest(), nullptr);
    EXPECT_EQ(customer->getCurrentOrder(), nullptr);
}

TEST_F(CustomerTest, GettersReturnCorrectValues) {
    EXPECT_EQ(customer->getName(), "Test Customer");
    EXPECT_EQ(customer->getId(), "TEST-001");
    EXPECT_DOUBLE_EQ(customer->getBudget(), 500.0);
}

// ============ Budget Management Tests ============

TEST_F(CustomerTest, CanAffordChecksCorrectly) {
    EXPECT_TRUE(customer->canAfford(100.0));
    EXPECT_TRUE(customer->canAfford(500.0));
    EXPECT_FALSE(customer->canAfford(501.0));
}

TEST_F(CustomerTest, DeductFromBudgetSuccess) {
    EXPECT_TRUE(customer->deductFromBudget(100.0));
    EXPECT_DOUBLE_EQ(customer->getBudget(), 400.0);
}

TEST_F(CustomerTest, DeductFromBudgetFailsWhenInsufficient) {
    EXPECT_FALSE(customer->deductFromBudget(600.0));
    EXPECT_DOUBLE_EQ(customer->getBudget(), 500.0); // Unchanged
}

TEST_F(CustomerTest, DeductFromBudgetRejectsNegativeAmount) {
    EXPECT_FALSE(customer->deductFromBudget(-50.0));
    EXPECT_DOUBLE_EQ(customer->getBudget(), 500.0);
}

TEST_F(CustomerTest, SetBudgetUpdatesCorrectly) {
    customer->setBudget(1000.0);
    EXPECT_DOUBLE_EQ(customer->getBudget(), 1000.0);
}

TEST_F(CustomerTest, SetBudgetRejectsNegativeValue) {
    customer->setBudget(-100.0);
    EXPECT_DOUBLE_EQ(customer->getBudget(), 500.0); // Unchanged
}

// ============ Cart Management via Mediator Tests ============

TEST_F(CustomerTest, AddPlantFromSalesFloorByName) {
    bool success = customer->addPlantFromSalesFloor("Rose");
    
    EXPECT_TRUE(success);
    EXPECT_EQ(customer->getCartSize(), 1);
}

TEST_F(CustomerTest, AddPlantFromSalesFloorByNameNotFound) {
    bool success = customer->addPlantFromSalesFloor("Orchid");
    
    EXPECT_FALSE(success);
    EXPECT_EQ(customer->getCartSize(), 0);
}

TEST_F(CustomerTest, AddPlantFromSalesFloorByPosition) {
    bool success = customer->addPlantFromSalesFloorPosition(0, 1);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(customer->getCartSize(), 1);
}

TEST_F(CustomerTest, AddPlantFromInvalidPosition) {
    bool success = customer->addPlantFromSalesFloorPosition(5, 5);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(customer->getCartSize(), 0);
}

TEST_F(CustomerTest, AddPlantFromEmptyPosition) {
    bool success = customer->addPlantFromSalesFloorPosition(2, 2);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(customer->getCartSize(), 0);
}

TEST_F(CustomerTest, AddMultiplePlantsToCart) {
    customer->addPlantFromSalesFloor("Rose");
    customer->addPlantFromSalesFloor("Daisy");
    
    EXPECT_EQ(customer->getCartSize(), 2);
}

TEST_F(CustomerTest, ReturnPlantToSalesFloor) {
    customer->addPlantFromSalesFloorPosition(0, 0);
    EXPECT_EQ(customer->getCartSize(), 1);
    
    bool success = customer->returnPlantToSalesFloor(0);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(customer->getCartSize(), 0);
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 3); // Back to 3
}

TEST_F(CustomerTest, ReturnPlantInvalidIndex) {
    customer->addPlantFromSalesFloorPosition(0, 0);
    
    bool success = customer->returnPlantToSalesFloor(5);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(customer->getCartSize(), 1); // Unchanged
}

TEST_F(CustomerTest, ClearCartRemovesAllItems) {
    customer->addPlantFromSalesFloor("Rose");
    customer->addPlantFromSalesFloor("Daisy");
    
    EXPECT_EQ(customer->getCartSize(), 2);
    
    customer->clearCart();
    
    EXPECT_EQ(customer->getCartSize(), 0);
}

TEST_F(CustomerTest, GetCartReturnsCorrectVector) {
    customer->addPlantFromSalesFloor("Rose");
    customer->addPlantFromSalesFloor("Daisy");
    
    std::vector<Plant*> cart = customer->getCart();
    
    EXPECT_EQ(cart.size(), 2);
}

// ============ Plant Decoration Tests ============

TEST_F(CustomerTest, GetPlantFromCartReturnsCorrectPlant) {
    customer->addPlantFromSalesFloor("Rose");
    
    Plant* plant = customer->getPlantFromCart(0);
    
    EXPECT_NE(plant, nullptr);
    EXPECT_EQ(plant->getName(), "Rose");
}

TEST_F(CustomerTest, GetPlantFromCartInvalidIndex) {
    customer->addPlantFromSalesFloor("Rose");
    
    Plant* plant = customer->getPlantFromCart(5);
    
    EXPECT_EQ(plant, nullptr);
}

TEST_F(CustomerTest, DecorateCartItemWithRibbon) {
    customer->addPlantFromSalesFloor("Rose");
    
    double originalPrice = customer->getPlantFromCart(0)->getPrice();
    customer->decorateCartItemWithRibbon(0);
    double newPrice = customer->getPlantFromCart(0)->getPrice();
    
    EXPECT_GT(newPrice, originalPrice);
    EXPECT_DOUBLE_EQ(newPrice, originalPrice + 15.0);
}

TEST_F(CustomerTest, DecorateCartItemWithGiftWrap) {
    customer->addPlantFromSalesFloor("Rose");
    
    double originalPrice = customer->getPlantFromCart(0)->getPrice();
    customer->decorateCartItemWithGiftWrap(0);
    double newPrice = customer->getPlantFromCart(0)->getPrice();
    
    EXPECT_GT(newPrice, originalPrice);
    EXPECT_DOUBLE_EQ(newPrice, originalPrice + 20.0);
}

TEST_F(CustomerTest, DecorateCartItemWithPot) {
    customer->addPlantFromSalesFloor("Rose");
    
    double originalPrice = customer->getPlantFromCart(0)->getPrice();
    customer->decorateCartItemWithPot(0, "blue");
    double newPrice = customer->getPlantFromCart(0)->getPrice();
    
    EXPECT_GT(newPrice, originalPrice);
    EXPECT_DOUBLE_EQ(newPrice, originalPrice + 80.0);
}

TEST_F(CustomerTest, DecorateInvalidCartIndex) {
    customer->addPlantFromSalesFloor("Rose");
    
    // Should not crash
    EXPECT_NO_THROW(customer->decorateCartItemWithRibbon(5));
    EXPECT_NO_THROW(customer->decorateCartItemWithGiftWrap(5));
    EXPECT_NO_THROW(customer->decorateCartItemWithPot(5, "red"));
}

TEST_F(CustomerTest, MultipleDecorationsStack) {
    customer->addPlantFromSalesFloor("Rose");
    
    double originalPrice = customer->getPlantFromCart(0)->getPrice();
    customer->decorateCartItemWithRibbon(0);
    customer->decorateCartItemWithGiftWrap(0);
    customer->decorateCartItemWithPot(0, "gold");
    double finalPrice = customer->getPlantFromCart(0)->getPrice();
    
    EXPECT_DOUBLE_EQ(finalPrice, originalPrice + 15.0 + 20.0 + 80.0);
}

// ============ Order Building Tests ============

TEST_F(CustomerTest, StartNewOrder) {
    customer->startNewOrder("Test Order");
    
    EXPECT_NE(customer->getCurrentOrder(), nullptr);
}

TEST_F(CustomerTest, StartNewOrderReplacesExisting) {
    customer->startNewOrder("Order 1");
    ConcreteOrder* firstOrder = customer->getCurrentOrder();
    
    customer->startNewOrder("Order 2");
    ConcreteOrder* secondOrder = customer->getCurrentOrder();
    
    EXPECT_NE(secondOrder, nullptr);
    // First order should have been deleted
}

TEST_F(CustomerTest, AddCartItemToOrder) {
    customer->addPlantFromSalesFloor("Rose");
    customer->startNewOrder("Test Order");
    customer->addCartItemToOrder(0);
    
    EXPECT_GT(customer->getCurrentOrder()->getPrice(), 0.0);
}

TEST_F(CustomerTest, AddCartItemToOrderWithoutActiveOrder) {
    customer->addPlantFromSalesFloor("Rose");
    
    // Should not crash
    EXPECT_NO_THROW(customer->addCartItemToOrder(0));
}

TEST_F(CustomerTest, AddCartItemToOrderInvalidIndex) {
    customer->startNewOrder("Test Order");
    
    // Should not crash
    EXPECT_NO_THROW(customer->addCartItemToOrder(5));
}

TEST_F(CustomerTest, AddEntireCartToOrder) {
    customer->addPlantFromSalesFloor("Rose");
    customer->addPlantFromSalesFloor("Daisy");
    customer->startNewOrder("Bulk Order");
    customer->addEntireCartToOrder();
    
    EXPECT_DOUBLE_EQ(customer->getCurrentOrder()->getPrice(), 80.0); // 50 + 30
}

TEST_F(CustomerTest, AddEntireCartToOrderWithEmptyCart) {
    customer->startNewOrder("Empty Order");
    
    // Should not crash
    EXPECT_NO_THROW(customer->addEntireCartToOrder());
}

TEST_F(CustomerTest, CalculateTotalReturnsCorrectAmount) {
    customer->addPlantFromSalesFloor("Rose");
    customer->addPlantFromSalesFloor("Daisy");
    customer->startNewOrder("Order");
    customer->addEntireCartToOrder();
    
    EXPECT_DOUBLE_EQ(customer->calculateTotal(), 80.0);
}

TEST_F(CustomerTest, CalculateTotalWithNoOrder) {
    EXPECT_DOUBLE_EQ(customer->calculateTotal(), 0.0);
}

// ============ Order Finalization Tests ============

TEST_F(CustomerTest, CreateFinalOrder) {
    customer->addPlantFromSalesFloor("Rose");
    customer->startNewOrder("Final Order");
    customer->addEntireCartToOrder();
    
    FinalOrder* finalOrder = customer->createFinalOrder();
    
    EXPECT_NE(finalOrder, nullptr);
    EXPECT_GT(finalOrder->calculateTotalPrice(), 0.0);
    
    delete finalOrder;
}

TEST_F(CustomerTest, CreateFinalOrderWithNoOrder) {
    FinalOrder* finalOrder = customer->createFinalOrder();
    
    EXPECT_EQ(finalOrder, nullptr);
}

TEST_F(CustomerTest, CreateFinalOrderResetsCurrentOrder) {
    customer->addPlantFromSalesFloor("Rose");
    customer->startNewOrder("Order");
    customer->addEntireCartToOrder();
    
    FinalOrder* finalOrder = customer->createFinalOrder();
    
    EXPECT_EQ(customer->getCurrentOrder(), nullptr);
    
    delete finalOrder;
}

// ============ Request Handling Tests ============

TEST_F(CustomerTest, CreateRequest) {
    Request* request = customer->createRequest("I need help");
    
    EXPECT_NE(request, nullptr);
    EXPECT_EQ(customer->getCurrentRequest(), request);
}

TEST_F(CustomerTest, CreateRequestReplacesExisting) {
    customer->createRequest("First request");
    customer->createRequest("Second request");
    
    EXPECT_NE(customer->getCurrentRequest(), nullptr);
    EXPECT_EQ(customer->getCurrentRequest()->getMessage(), "Second request");
}

TEST_F(CustomerTest, SubmitRequestToStaff) {
    SalesAssistant* assistant = new SalesAssistant(mediator, "Assistant", "SA-001");
    mediator->registerColleague(assistant);
    
    customer->createRequest("I want a Rose");
    
    // Should not crash
    EXPECT_NO_THROW(customer->submitRequestToStaff(assistant));
    
    delete assistant;
}

TEST_F(CustomerTest, SubmitRequestWithNullHandler) {
    customer->createRequest("Test request");
    
    // Should not crash
    EXPECT_NO_THROW(customer->submitRequestToStaff(nullptr));
}

TEST_F(CustomerTest, SubmitRequestWithNoCurrentRequest) {
    SalesAssistant* assistant = new SalesAssistant(mediator, "Assistant", "SA-001");
    
    // Should not crash
    EXPECT_NO_THROW(customer->submitRequestToStaff(assistant));
    
    delete assistant;
}

TEST_F(CustomerTest, ReceiveResponse) {
    // Should not crash
    EXPECT_NO_THROW(customer->receiveResponse("Your request has been processed"));
}

// ============ Derived Customer Types Tests ============

TEST(DerivedCustomerTest, RegularCustomerInitialization) {
    RegularCustomer customer;
    
    EXPECT_EQ(customer.getName(), "Regular Customer");
    EXPECT_EQ(customer.getId(), "REG-001");
    EXPECT_DOUBLE_EQ(customer.getBudget(), 300.0);
}

TEST(DerivedCustomerTest, CorporateCustomerInitialization) {
    CorporateCustomer customer;
    
    EXPECT_EQ(customer.getName(), "Corporate Client");
    EXPECT_EQ(customer.getId(), "CORP-001");
    EXPECT_DOUBLE_EQ(customer.getBudget(), 1000.0);
}

TEST(DerivedCustomerTest, WalkInCustomerInitialization) {
    WalkInCustomer customer;
    
    EXPECT_EQ(customer.getName(), "Walk-In Customer");
    EXPECT_EQ(customer.getId(), "WALK-001");
    EXPECT_DOUBLE_EQ(customer.getBudget(), 150.0);
}

TEST(DerivedCustomerTest, BudgetHierarchy) {
    WalkInCustomer walkIn;
    RegularCustomer regular;
    CorporateCustomer corporate;
    
    EXPECT_LT(walkIn.getBudget(), regular.getBudget());
    EXPECT_LT(regular.getBudget(), corporate.getBudget());
}

TEST(DerivedCustomerTest, CheckoutWithEmptyCart) {
    RegularCustomer customer;
    
    // Should handle empty cart gracefully
    EXPECT_NO_THROW(customer.checkOut());
}

// ============ Edge Cases and Null Safety Tests ============

TEST_F(CustomerTest, AddToCartWithNullPlant) {
    // This is a private method, but we can test indirectly
    // by trying to add from an empty position
    bool success = customer->addPlantFromSalesFloorPosition(2, 2);
    EXPECT_FALSE(success);
}

TEST_F(CustomerTest, GetPlantFromEmptyCart) {
    Plant* plant = customer->getPlantFromCart(0);
    EXPECT_EQ(plant, nullptr);
}

TEST_F(CustomerTest, DecorateWithEmptyCart) {
    EXPECT_NO_THROW(customer->decorateCartItemWithRibbon(0));
    EXPECT_NO_THROW(customer->decorateCartItemWithGiftWrap(0));
    EXPECT_NO_THROW(customer->decorateCartItemWithPot(0, "blue"));
}

TEST_F(CustomerTest, ClearEmptyCart) {
    EXPECT_NO_THROW(customer->clearCart());
    EXPECT_EQ(customer->getCartSize(), 0);
}

TEST_F(CustomerTest, MultipleOperationsOnSameCart) {
    customer->addPlantFromSalesFloor("Rose");
    customer->decorateCartItemWithRibbon(0);
    customer->startNewOrder("Order");
    customer->addCartItemToOrder(0);
    
    // Cart still owns the plant
    EXPECT_EQ(customer->getCartSize(), 1);
    
    FinalOrder* finalOrder = customer->createFinalOrder();
    EXPECT_NE(finalOrder, nullptr);
    
    delete finalOrder;
}

// ============ Integration Tests ============

TEST_F(CustomerTest, CompleteShoppingFlow) {
    // 1. Add plants to cart
    customer->addPlantFromSalesFloor("Rose");
    customer->addPlantFromSalesFloor("Daisy");
    EXPECT_EQ(customer->getCartSize(), 2);
    
    // 2. Decorate plants
    customer->decorateCartItemWithRibbon(0);
    customer->decorateCartItemWithGiftWrap(1);
    
    // 3. Build order
    customer->startNewOrder("Complete Order");
    customer->addEntireCartToOrder();
    
    // 4. Finalize order
    FinalOrder* finalOrder = customer->createFinalOrder();
    EXPECT_NE(finalOrder, nullptr);
    
    double total = finalOrder->calculateTotalPrice();
    EXPECT_GT(total, 0.0);
    
    // 5. Check budget
    EXPECT_TRUE(customer->canAfford(total));
    
    // 6. Process payment
    EXPECT_TRUE(customer->deductFromBudget(total));
    
    // 7. Clear cart
    customer->clearCart();
    EXPECT_EQ(customer->getCartSize(), 0);
    
    delete finalOrder;
}