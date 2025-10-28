#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>

// ============ Implementation headers ============
#include "include/Plant.h"
#include "include/CareStrategy.h"
#include "include/PlantState.h"
#include "include/SeedlingState.h"
#include "include/GrowingState.h"
#include "include/MatureState.h"
#include "include/FloweringState.h"
#include "include/DeadState.h"
#include "include/SucculentCareStrategy.h"
#include "include/FlowerCareStrategy.h"
#include "include/VegetableCareStrategy.h"
#include "include/OtherPlantCareStrategy.h"

// ============ Decorator headers ============
#include "include/Decorator.h"
#include "include/RibbonDecorator.h"
#include "include/GiftWrapDecorator.h"
#include "include/DecorativePotDecorator.h"

// ============ Composite headers ============
#include "include/Order.h"
#include "include/Leaf.h"
#include "include/ConcreteOrder.h"

// ============ Composite test structure ============
class CompositeTest : public ::testing::Test {
protected:
    Plant* plant1;
    Plant* plant2;
    Plant* plant3;
    
    void SetUp() override {
        plant1 = new Plant("Rose", "R001", 
                          new FlowerCareStrategy(), 
                          new MatureState());
        plant1->setPrice(50.0);
        
        plant2 = new Plant("Tulip", "T001", 
                          new FlowerCareStrategy(), 
                          new MatureState());
        plant2->setPrice(30.0);
        
        plant3 = new Plant("Daisy", "D001", 
                          new FlowerCareStrategy(), 
                          new MatureState());
        plant3->setPrice(20.0);
    }
    
    void TearDown() override {
        // Plants will be deleted by Leaf destructors
    }
};

// ============ COMPOSITE Tests ============

//Tests single leaf returns the correct price
TEST_F(CompositeTest, SingleLeafReturnsCorrectPrice) {
    Leaf* leaf = new Leaf(plant1);
    
    EXPECT_DOUBLE_EQ(leaf->getPrice(), 50.0);
    
    delete leaf;
}

//Tests single leaf returns the correct description
TEST_F(CompositeTest, SingleLeafReturnsCorrectDescription) {
    Leaf* leaf = new Leaf(plant1);
    
    std::string desc = leaf->description();
    EXPECT_NE(desc.find("Rose"), std::string::npos);
    
    delete leaf;
}

//Tests you can't add a leaf to a leaf
TEST_F(CompositeTest, LeafThrowsExceptionOnAdd) {
    Leaf* leaf = new Leaf(plant1);
    Leaf* anotherLeaf = new Leaf(plant2);
    
    EXPECT_THROW(leaf->add(anotherLeaf), std::runtime_error);
    
    delete leaf;
    delete anotherLeaf;
}

//Tests you can't remove from a leaf
TEST_F(CompositeTest, LeafThrowsExceptionOnRemove) {
    Leaf* leaf = new Leaf(plant1);
    
    EXPECT_THROW(leaf->remove(nullptr), std::runtime_error);
    
    delete leaf;
}

//Tests empty concrete order has a price of zero
TEST_F(CompositeTest, EmptyOrderHasZeroPrice) {
    ConcreteOrder* order = new ConcreteOrder("Empty Order");
    
    EXPECT_DOUBLE_EQ(order->getPrice(), 0.0);
    
    delete order;
}

//Tests concrete order with a single leaf returns correct price
TEST_F(CompositeTest, OrderWithSingleLeafCorrectPrice) {
    ConcreteOrder* order = new ConcreteOrder("Simple Order");
    order->add(new Leaf(plant1));
    
    EXPECT_DOUBLE_EQ(order->getPrice(), 50.0);
    
    delete order;
}

//Tests concrete order with multiple leaves returns the correct price
TEST_F(CompositeTest, OrderWithMultipleLeavesCorrectPrice) {
    ConcreteOrder* order = new ConcreteOrder("Multi-Plant Order");
    order->add(new Leaf(plant1));
    order->add(new Leaf(plant2));
    order->add(new Leaf(plant3));
    
    // 50 + 30 + 20 = 100
    EXPECT_DOUBLE_EQ(order->getPrice(), 100.0);
    
    delete order;
}

//Tests nested concrete orders return the correct price
TEST_F(CompositeTest, NestedOrdersCorrectPrice) {
    ConcreteOrder* mainOrder = new ConcreteOrder("Main Order");
    ConcreteOrder* subOrder1 = new ConcreteOrder("Sub Order 1");
    ConcreteOrder* subOrder2 = new ConcreteOrder("Sub Order 2");
    
    subOrder1->add(new Leaf(plant1)); // 50
    subOrder2->add(new Leaf(plant2)); // 30
    subOrder2->add(new Leaf(plant3)); // 20
    
    mainOrder->add(subOrder1);
    mainOrder->add(subOrder2);
    
    // 50 + 30 + 20 = 100
    EXPECT_DOUBLE_EQ(mainOrder->getPrice(), 100.0);
    
    delete mainOrder;
}

//Tests removing a leaf from concrete order correctly updates the price
TEST_F(CompositeTest, RemoveFromOrderUpdatesPrice) {
    ConcreteOrder* order = new ConcreteOrder("Order");
    Leaf* leaf1 = new Leaf(plant1);
    Leaf* leaf2 = new Leaf(plant2);
    
    order->add(leaf1);
    order->add(leaf2);
    
    EXPECT_DOUBLE_EQ(order->getPrice(), 80.0); // 50 + 30
    
    order->remove(leaf1);
    
    EXPECT_DOUBLE_EQ(order->getPrice(), 30.0); // Only plant2
    
    delete leaf1; // We removed it, so we need to delete it manually
    delete order;
}

//Tests order description contains all items
TEST_F(CompositeTest, OrderDescriptionIncludesAllLeaves) {
    ConcreteOrder* order = new ConcreteOrder("Flower Bouquet");
    order->add(new Leaf(plant1));
    order->add(new Leaf(plant2));
    
    std::string desc = order->description();
    EXPECT_NE(desc.find("Rose"), std::string::npos);
    EXPECT_NE(desc.find("Tulip"), std::string::npos);
    EXPECT_NE(desc.find("Flower Bouquet"), std::string::npos);
    
    delete order;
}

//Tests cloning concrete orders
TEST_F(CompositeTest, CloneCreatesDeepCopy) {
    ConcreteOrder* original = new ConcreteOrder("Original");
    Plant* p = new Plant("Rose", "R001", 
                        new FlowerCareStrategy(), 
                        new MatureState());
    p->setPrice(50.0);
    original->add(new Leaf(p));
    
    ConcreteOrder* cloned = dynamic_cast<ConcreteOrder*>(original->clone());
    ASSERT_NE(cloned, nullptr);
    
    EXPECT_DOUBLE_EQ(original->getPrice(), cloned->getPrice());
    EXPECT_NE(original, cloned); // Different objects
    
    delete original;
    delete cloned;
}

//Tests decorated plants in a composite
TEST_F(CompositeTest, DecoratedPlantInOrderCorrectPrice) {
    Plant* decoratedPlant = new RibbonDecorator(plant1);
    
    ConcreteOrder* order = new ConcreteOrder("Decorated Order");
    order->add(new Leaf(decoratedPlant));
    
    // 50 (base) + 15 (ribbon) = 65
    EXPECT_DOUBLE_EQ(order->getPrice(), 65.0);
    
    delete order;
}

//Tests order's getName returns the correct name
TEST_F(CompositeTest, OrderGetNameReturnsCorrectName) {
    ConcreteOrder* order = new ConcreteOrder("Test Order");
    
    EXPECT_EQ(order->getName(), "Test Order");
    
    delete order;
}

//Tests wrapped leaf returns correct plant name
TEST_F(CompositeTest, LeafGetNameReturnsPlantName) {
    Leaf* leaf = new Leaf(plant1);
    
    EXPECT_EQ(leaf->getName(), "Rose");
    
    delete leaf;
}

//Tests deep nested orders calculate prices correctly
TEST_F(CompositeTest, DeepNestingCalculatesCorrectly) {
    ConcreteOrder* level1 = new ConcreteOrder("Level 1");
    ConcreteOrder* level2 = new ConcreteOrder("Level 2");
    ConcreteOrder* level3 = new ConcreteOrder("Level 3");
    
    level3->add(new Leaf(plant1)); // 50
    level2->add(level3);
    level2->add(new Leaf(plant2)); // 30
    level1->add(level2);
    level1->add(new Leaf(plant3)); // 20
    
    // Total: 50 + 30 + 20 = 100
    EXPECT_DOUBLE_EQ(level1->getPrice(), 100.0);
    
    delete level1;
}

