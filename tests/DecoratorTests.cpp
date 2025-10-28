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

// ============ DECORATOR Tests ============

class DecoratorTest : public ::testing::Test {
protected:
    Plant* basePlant;
    
    void SetUp() override {
        // Create a base plant with mature state so it has a reasonable price
        basePlant = new Plant("Rose", "R001", 
                              new FlowerCareStrategy(), 
                              new MatureState());
        basePlant->setPrice(50.0);
    }
    
    void TearDown() override {
        // Note: Decorators delete the wrapped plant in their destructor
        // So we only delete basePlant if it wasn't wrapped in a test
    }
};

//Tests plant decorated with ribbon calculates price correctly
TEST_F(DecoratorTest, RibbonDecoratorAddsCorrectPrice) {
    Plant* decoratedPlant = new RibbonDecorator(basePlant);
    
    EXPECT_DOUBLE_EQ(decoratedPlant->getPrice(), 65.0); // 50 + 15
    
    delete decoratedPlant;
}

//Tests plant decorated with ribbon returns the correct description
TEST_F(DecoratorTest, RibbonDecoratorAddsCorrectDescription) {
    Plant* decoratedPlant = new RibbonDecorator(basePlant);
    
    std::string desc = decoratedPlant->description();
    EXPECT_NE(desc.find("Rose"), std::string::npos);
    EXPECT_NE(desc.find("ribbon"), std::string::npos);
    
    delete decoratedPlant;
}

//Tests plant decorated with gift wrap calculates price correctly
TEST_F(DecoratorTest, GiftWrapDecoratorAddsCorrectPrice) {
    Plant* decoratedPlant = new GiftWrapDecorator(basePlant);
    
    EXPECT_DOUBLE_EQ(decoratedPlant->getPrice(), 70.0); // 50 + 20
    
    delete decoratedPlant;
}

//Tests plant decorated with gift wrap returns the correct description
TEST_F(DecoratorTest, GiftWrapDecoratorAddsCorrectDescription) {
    Plant* decoratedPlant = new GiftWrapDecorator(basePlant);
    
    std::string desc = decoratedPlant->description();
    EXPECT_NE(desc.find("Rose"), std::string::npos);
    EXPECT_NE(desc.find("gift wrapping"), std::string::npos);
    
    delete decoratedPlant;
}

//Tests plant decorated with pot calculates price correctly
TEST_F(DecoratorTest, DecorativePotDecoratorAddsCorrectPrice) {
    Plant* decoratedPlant = new DecorativePotDecorator(basePlant, "blue");
    
    EXPECT_DOUBLE_EQ(decoratedPlant->getPrice(), 130.0); // 50 + 80
    
    delete decoratedPlant;
}

//Tests plant decorated with pot returns the correct description with the colour
TEST_F(DecoratorTest, DecorativePotDecoratorAddsCorrectDescriptionWithColor) {
    Plant* decoratedPlant = new DecorativePotDecorator(basePlant, "terracotta");
    
    std::string desc = decoratedPlant->description();
    EXPECT_NE(desc.find("Rose"), std::string::npos);
    EXPECT_NE(desc.find("terracotta"), std::string::npos);
    EXPECT_NE(desc.find("pot"), std::string::npos);
    
    delete decoratedPlant;
}

//Tests multiple decorators on one plant - correct price
TEST_F(DecoratorTest, StackedDecoratorsAddCorrectPrice) {
    Plant* decoratedPlant = new GiftWrapDecorator(
        new RibbonDecorator(basePlant)
    );
    
    // 50 (base) + 15 (ribbon) + 20 (gift wrap) = 85
    EXPECT_DOUBLE_EQ(decoratedPlant->getPrice(), 85.0);
    
    delete decoratedPlant;
}

//Tests multiple decorators on one plant - correct description
TEST_F(DecoratorTest, StackedDecoratorsAccumulateDescriptions) {
    Plant* decoratedPlant = new GiftWrapDecorator(
        new RibbonDecorator(basePlant)
    );
    
    std::string desc = decoratedPlant->description();
    EXPECT_NE(desc.find("Rose"), std::string::npos);
    EXPECT_NE(desc.find("ribbon"), std::string::npos);
    EXPECT_NE(desc.find("gift wrapping"), std::string::npos);
    
    delete decoratedPlant;
}

//Tests plant with all 3 decorators - correct price
TEST_F(DecoratorTest, TripleStackedDecoratorsCorrectPrice) {
    Plant* decoratedPlant = new DecorativePotDecorator(
        new GiftWrapDecorator(
            new RibbonDecorator(basePlant)
        ),
        "red"
    );
    
    // 50 + 15 + 20 + 80 = 165
    EXPECT_DOUBLE_EQ(decoratedPlant->getPrice(), 165.0);
    
    delete decoratedPlant;
}

//Tests plant with all 3 decorators - correct description
TEST_F(DecoratorTest, TripleStackedDecoratorsAllDescriptions) {
    Plant* decoratedPlant = new DecorativePotDecorator(
        new GiftWrapDecorator(
            new RibbonDecorator(basePlant)
        ),
        "gold" //pot colour
    );
    
    std::string desc = decoratedPlant->description();
    EXPECT_NE(desc.find("Rose"), std::string::npos);
    EXPECT_NE(desc.find("ribbon"), std::string::npos);
    EXPECT_NE(desc.find("gift wrapping"), std::string::npos);
    EXPECT_NE(desc.find("gold"), std::string::npos);
    EXPECT_NE(desc.find("pot"), std::string::npos);
    
    delete decoratedPlant;
}

//Tests plant with base price 0.0
TEST_F(DecoratorTest, DecoratorOnZeroPricePlant) {
    Plant* freePlant = new Plant("Freebie", "F001", 
                                 new OtherPlantCareStrategy(), 
                                 new SeedlingState());
    freePlant->setPrice(0.0);
    
    Plant* decorated = new RibbonDecorator(freePlant);
    
    EXPECT_DOUBLE_EQ(decorated->getPrice(), 15.0);
    
    delete decorated;
}

//Testing different decorator orders give you the same price
TEST_F(DecoratorTest, DecoratorOrderDoesNotAffectPrice) {
    Plant* plant1 = new Plant("Rose1", "R001", 
                             new FlowerCareStrategy(), 
                             new MatureState());
    plant1->setPrice(50.0);
    
    Plant* plant2 = new Plant("Rose2", "R002", 
                             new FlowerCareStrategy(), 
                             new MatureState());
    plant2->setPrice(50.0);
    
    Plant* decorated1 = new RibbonDecorator(new GiftWrapDecorator(plant1));
    Plant* decorated2 = new GiftWrapDecorator(new RibbonDecorator(plant2));
    
    EXPECT_DOUBLE_EQ(decorated1->getPrice(), decorated2->getPrice());
    
    delete decorated1;
    delete decorated2;
}

//Tests decorator doesn't alter plant name
TEST_F(DecoratorTest, DecoratorPreservesPlantName) {
    Plant* decoratedPlant = new RibbonDecorator(basePlant);
    
    EXPECT_EQ(decoratedPlant->getName(), "Rose");
    
    delete decoratedPlant;
}

//Tests dailyUpdate propegates to wrapped plants
TEST_F(DecoratorTest, DailyUpdatePropagatesThroughDecorators) {
    Plant* decoratedPlant = new RibbonDecorator(basePlant);
    
    int initialAge = decoratedPlant->getAge();
    decoratedPlant->dailyUpdate();
    
    EXPECT_EQ(decoratedPlant->getAge(), initialAge + 1);
    
    delete decoratedPlant;
}

//Tests different pot colours appear correctly in descriptions
TEST_F(DecoratorTest, DifferentPotColorsInDescription) {
    Plant* plant1 = new Plant("Cactus1", "C001", 
                             new SucculentCareStrategy(), 
                             new MatureState());
    plant1->setPrice(25.0);
    
    Plant* plant2 = new Plant("Cactus2", "C002", 
                             new SucculentCareStrategy(), 
                             new MatureState());
    plant2->setPrice(25.0);
    
    Plant* bluePot = new DecorativePotDecorator(plant1, "blue");
    Plant* redPot = new DecorativePotDecorator(plant2, "red");
    
    EXPECT_NE(bluePot->description().find("blue"), std::string::npos);
    EXPECT_NE(redPot->description().find("red"), std::string::npos);
    
    delete bluePot;
    delete redPot;
}
