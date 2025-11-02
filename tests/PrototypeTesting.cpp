
#include <gtest/gtest.h>
#include "include/FinalOrder.h"
#include "include/ConcreteOrder.h"


class PrototypeTest : public ::testing::Test {
protected:
    FinalOrder* original;

    void SetUp() override {
        original = new FinalOrder("John Doe");

        // Create sample composite orders
        ConcreteOrder* order1 = new ConcreteOrder("Flower Pack");
        ConcreteOrder* order2 = new ConcreteOrder("Succulent Set");

        // Add to final order
        original->addOrder(order1);
        original->addOrder(order2);
    }

    void TearDown() override {
        delete original;
    }
};


TEST_F(PrototypeTest, CloneCreatesIndependentObject) {
    FinalOrder* clone = original->clone();
    EXPECT_NE(clone, original); // different memory
    delete clone;
}


TEST_F(PrototypeTest, CloneHasIdenticalData) {
    FinalOrder* clone = original->clone();
    EXPECT_EQ(clone->getSummary(), original->getSummary());
    delete clone;
}


TEST_F(PrototypeTest, CloneIsIndependent) {
    FinalOrder* clone = original->clone();
    ConcreteOrder* newOrder = new ConcreteOrder("Cactus Set");
    clone->addOrder(newOrder);

    // Instead of relying on numeric totals (not implemented), check summary size change
    EXPECT_NE(clone->getSummary().size(), original->getSummary().size())
        << "Cloned and original orders should differ after modification.";

    delete clone;
}

/**
 * @test Ensures that multiple clones remain consistent and independent.
 */
TEST_F(PrototypeTest, MultipleClonesRemainConsistent) {
    FinalOrder* clone1 = original->clone();
    FinalOrder* clone2 = original->clone();

    EXPECT_EQ(clone1->getSummary(), clone2->getSummary());
    EXPECT_NE(clone1, clone2); // distinct memory

    delete clone1;
    delete clone2;
}
