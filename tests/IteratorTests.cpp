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

// ============ Iterator headers ============
#include "include/Iterator.h"
#include "include/ConcreteIterator.h"

// ============ Iterator test structure ============
class IteratorTest : public ::testing::Test {
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
};

// ============ Iterator Tests ============

//Tests iteration on a single leaf
TEST_F(IteratorTest, IteratorOnSingleLeaf) {
    Leaf* leaf = new Leaf(plant1);
    Iterator* it = leaf->createIterator();
    
    EXPECT_FALSE(it->isDone());
    EXPECT_NE(it->currentItem(), nullptr);
    
    it->next();
    EXPECT_TRUE(it->isDone());
    
    delete it;
    delete leaf;
}

//Tests iteration on an empty order
TEST_F(IteratorTest, IteratorOnEmptyOrder) {
    ConcreteOrder* order = new ConcreteOrder("Empty");
    Iterator* it = order->createIterator();
    
    EXPECT_TRUE(it->isDone());
    EXPECT_EQ(it->currentItem(), nullptr);
    
    delete it;
    delete order;
}

//Tests iterator traverses over all leaves
TEST_F(IteratorTest, IteratorTraversesAllLeaves) {
    ConcreteOrder* order = new ConcreteOrder("Order");
    order->add(new Leaf(plant1));
    order->add(new Leaf(plant2));
    order->add(new Leaf(plant3));
    
    Iterator* it = order->createIterator();
    
    int count = 0;
    for (it->first(); !it->isDone(); it->next()) {
        EXPECT_NE(it->currentItem(), nullptr);
        count++;
    }
    
    EXPECT_EQ(count, 3);
    
    delete it;
    delete order;
}

//Tests iterator flattens nested orders
TEST_F(IteratorTest, IteratorFlattensNestedOrders) {
    ConcreteOrder* mainOrder = new ConcreteOrder("Main");
    ConcreteOrder* subOrder = new ConcreteOrder("Sub");
    
    subOrder->add(new Leaf(plant1));
    subOrder->add(new Leaf(plant2));
    
    mainOrder->add(subOrder);
    mainOrder->add(new Leaf(plant3));
    
    Iterator* it = mainOrder->createIterator();
    
    int count = 0;
    for (it->first(); !it->isDone(); it->next()) {
        count++;
    }
    
    EXPECT_EQ(count, 3); // Should find all 3 leaves
    
    delete it;
    delete mainOrder;
}

//Tests iterator first() resets the position
TEST_F(IteratorTest, FirstResetsIteratorPosition) {
    ConcreteOrder* order = new ConcreteOrder("Order");
    order->add(new Leaf(plant1));
    order->add(new Leaf(plant2));
    
    Iterator* it = order->createIterator();
    
    it->next();
    it->next();
    EXPECT_TRUE(it->isDone());
    
    it->first();
    EXPECT_FALSE(it->isDone());
    
    delete it;
    delete order;
}

//Tests currentItem() returns correct item in the order
TEST_F(IteratorTest, CurrentItemReturnsCorrectLeaf) {
    ConcreteOrder* order = new ConcreteOrder("Order");
    Leaf* leaf1 = new Leaf(plant1);
    Leaf* leaf2 = new Leaf(plant2);
    
    order->add(leaf1);
    order->add(leaf2);
    
    Iterator* it = order->createIterator();
    
    Order* first = it->currentItem();
    EXPECT_EQ(first, leaf1);
    
    it->next();
    Order* second = it->currentItem();
    EXPECT_EQ(second, leaf2);
    
    delete it;
    delete order;
}

//Tests iterator after last item returns null
TEST_F(IteratorTest, CurrentItemAfterLastReturnsNull) {
    Leaf* leaf = new Leaf(plant1);
    Iterator* it = leaf->createIterator();
    
    it->next();
    EXPECT_TRUE(it->isDone());
    EXPECT_EQ(it->currentItem(), nullptr);
    
    delete it;
    delete leaf;
}

//Tests multiple iterators are independent
TEST_F(IteratorTest, MultipleIteratorsIndependent) {
    ConcreteOrder* order = new ConcreteOrder("Order");
    order->add(new Leaf(plant1));
    order->add(new Leaf(plant2));
    
    Iterator* it1 = order->createIterator();
    Iterator* it2 = order->createIterator();
    
    it1->next();
    
    EXPECT_NE(it1->currentItem(), it2->currentItem());
    
    delete it1;
    delete it2;
    delete order;
}

//Tests iterator can calculate total price
TEST_F(IteratorTest, IteratorCanCalculateTotalPrice) {
    ConcreteOrder* order = new ConcreteOrder("Order");
    order->add(new Leaf(plant1)); // 50
    order->add(new Leaf(plant2)); // 30
    order->add(new Leaf(plant3)); // 20
    
    Iterator* it = order->createIterator();
    
    double total = 0.0;
    for (it->first(); !it->isDone(); it->next()) {
        Order* item = it->currentItem();
        if (item) {
            total += item->getPrice();
        }
    }
    
    EXPECT_DOUBLE_EQ(total, 100.0);
    
    delete it;
    delete order;
}

//Tests iterator with decorated plants
TEST_F(IteratorTest, IteratorWithDecoratedPlants) {
    Plant* decorated = new RibbonDecorator(plant1);
    
    ConcreteOrder* order = new ConcreteOrder("Decorated Order");
    order->add(new Leaf(decorated));
    order->add(new Leaf(plant2));
    
    Iterator* it = order->createIterator();
    
    double total = 0.0;
    int count = 0;
    for (it->first(); !it->isDone(); it->next()) {
        Order* item = it->currentItem();
        if (item) {
            total += item->getPrice();
            count++;
        }
    }
    
    EXPECT_EQ(count, 2);
    EXPECT_DOUBLE_EQ(total, 95.0); // 65 (50+15) + 30
    
    delete it;
    delete order;
}

//Tests iterator on deeply nested structure
TEST_F(IteratorTest, IteratorOnDeeplyNestedStructure) {
    ConcreteOrder* level1 = new ConcreteOrder("Level 1");
    ConcreteOrder* level2 = new ConcreteOrder("Level 2");
    ConcreteOrder* level3 = new ConcreteOrder("Level 3");
    
    level3->add(new Leaf(plant1));
    level2->add(level3);
    level2->add(new Leaf(plant2));
    level1->add(level2);
    level1->add(new Leaf(plant3));
    
    Iterator* it = level1->createIterator();
    
    int count = 0;
    for (it->first(); !it->isDone(); it->next()) {
        count++;
    }
    
    EXPECT_EQ(count, 3);
    
    delete it;
    delete level1;
}

//Tests iterator does not modify order
TEST_F(IteratorTest, IteratorDoesNotModifyOrder) {
    ConcreteOrder* order = new ConcreteOrder("Order");
    order->add(new Leaf(plant1));
    order->add(new Leaf(plant2));
    
    double priceBefore = order->getPrice();
    
    Iterator* it = order->createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        // Just iterate, don't modify
    }
    
    double priceAfter = order->getPrice();
    EXPECT_DOUBLE_EQ(priceBefore, priceAfter);
    
    delete it;
    delete order;
}