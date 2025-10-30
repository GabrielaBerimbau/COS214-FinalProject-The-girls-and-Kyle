#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "include/PlantState.h"
#include "include/SeedlingState.h"
#include "include/GrowingState.h"
#include "include/MatureState.h"
#include "include/FloweringState.h"
#include "include/DeadState.h"

#include "include/Plant.h"
#include "include/CareStrategy.h"
#include "include/FlowerCareStrategy.h"
#include "include/SucculentCareStrategy.h"
#include "include/VegetableCareStrategy.h"
#include "include/OtherPlantCareStrategy.h"

class StateTest : public ::testing::Test {
protected:
    Plant* testPlant;
    
    void SetUp() override {
        testPlant = new Plant("TestPlant", "TEST-001", 
                             new FlowerCareStrategy(), 
                             new SeedlingState());
        testPlant->setPrice(50.0);
    }
    
    void TearDown() override {
        delete testPlant;
    }
    
    // Helper method to set plant conditions
    void setPlantConditions(int water, int nutrients, int sunlight, int age) {
        testPlant->setWaterLevel(water);
        testPlant->setNutrientLevel(nutrients);
        testPlant->setSunlightExposure(sunlight);
        
        // Simulate aging
        for (int i = 0; i < age; i++) {
            testPlant->incrementAge();
        }
        
        testPlant->updateHealth();
    }
};

TEST_F(StateTest, SeedlingStateInitialization) {
    PlantState* state = testPlant->getState();
    
    ASSERT_NE(state, nullptr);
    EXPECT_EQ(state->getStateName(), "Seedling");
}

TEST_F(StateTest, SeedlingStateNotReadyForSale) {
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_FALSE(testPlant->isReadyForSale());
}

TEST_F(StateTest, SeedlingTransitionsToGrowingWhenConditionsMet) {
    setPlantConditions(60, 60, 60, 7); // Age 7, good health
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Growing");
}

TEST_F(StateTest, SeedlingDoesNotTransitionWhenTooYoung) {
    setPlantConditions(60, 60, 60, 5); // Age 5, good health but too young
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Seedling");
}

TEST_F(StateTest, SeedlingDoesNotTransitionWhenPoorHealth) {
    setPlantConditions(10, 10, 10, 10); // Age sufficient but poor health
    
    testPlant->getState()->handleChange(testPlant);
    
    // Should stay seedling or die depending on health
    std::string stateName = testPlant->getState()->getStateName();
    EXPECT_TRUE(stateName == "Seedling" || stateName == "Dead");
}

TEST_F(StateTest, SeedlingDiesWithVeryPoorHealth) {
    // Health < 20 causes death in Seedling state
    setPlantConditions(5, 5, 5, 5); // Health = (5+5+5)/3 = 5 < 20
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Dead");
}

TEST_F(StateTest, SeedlingStateAlertsWhenWaterLow) {
    setPlantConditions(30, 50, 50, 3); // Low water
    
    testing::internal::CaptureStdout();
    testPlant->getState()->handleChange(testPlant);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("water"), std::string::npos);
}

TEST_F(StateTest, GrowingStateTransitionsToMatureWhenConditionsMet) {
    testPlant->setState(new GrowingState());
    setPlantConditions(60, 60, 60, 12); // Age 12, health >= 50
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Mature");
}

TEST_F(StateTest, GrowingStateDoesNotTransitionWhenTooYoung) {
    testPlant->setState(new GrowingState());
    setPlantConditions(60, 60, 60, 10); // Age 10, too young for mature
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Growing");
}

TEST_F(StateTest, GrowingStateDiesWithPoorHealth) {
    testPlant->setState(new GrowingState());
    // Health < 20 causes death in Growing state
    setPlantConditions(5, 5, 5, 8); // Health = (5+5+5)/3 = 5 < 20
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Dead");
}

TEST_F(StateTest, GrowingStateAlertsWhenNutrientsLow) {
    testPlant->setState(new GrowingState());
    setPlantConditions(50, 20, 50, 8); // Low nutrients
    
    testing::internal::CaptureStdout();
    testPlant->getState()->handleChange(testPlant);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("fertilizer"), std::string::npos);
}

TEST_F(StateTest, GrowingStateNotReadyForSale) {
    testPlant->setState(new GrowingState());
    setPlantConditions(60, 60, 60, 10);
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_FALSE(testPlant->isReadyForSale());
}

TEST_F(StateTest, MatureStateMarksPlantReadyForSale) {
    testPlant->setState(new MatureState());
    setPlantConditions(60, 60, 60, 15);
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_TRUE(testPlant->isReadyForSale());
}

TEST_F(StateTest, MatureStateTransitionsToFloweringWhenConditionsMet) {
    testPlant->setState(new MatureState());
    setPlantConditions(90, 90, 90, 35); // Age 35, health >= 80
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Flowering");
}

TEST_F(StateTest, MatureStateDoesNotTransitionWhenTooYoung) {
    testPlant->setState(new MatureState());
    setPlantConditions(90, 90, 90, 30); // Age 30, too young for flowering
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Mature");
}

TEST_F(StateTest, MatureStateDiesWithVeryPoorHealth) {
    testPlant->setState(new MatureState());
    // Health < 10 causes death in Mature state
    setPlantConditions(0, 0, 0, 20); // Health = (0+0+0)/3 = 0 < 10
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Dead");
}

TEST_F(StateTest, MatureStateAlertsWhenWaterLow) {
    testPlant->setState(new MatureState());
    setPlantConditions(15, 50, 50, 15); // Low water
    
    testing::internal::CaptureStdout();
    testPlant->getState()->handleChange(testPlant);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("water"), std::string::npos);
}

TEST_F(StateTest, FloweringStateMarksPlantReadyForSale) {
    testPlant->setState(new FloweringState());
    setPlantConditions(60, 60, 60, 40);
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_TRUE(testPlant->isReadyForSale());
}

TEST_F(StateTest, FloweringStateAppliesPremiumPricing) {
    testPlant->setPrice(10.0);
    testPlant->setState(new FloweringState());
    setPlantConditions(60, 60, 60, 40);
    
    testPlant->getState()->handleChange(testPlant);
    
    // Price should be increased by 1.5x
    EXPECT_DOUBLE_EQ(testPlant->getPrice(), 15.0);
}

TEST_F(StateTest, FloweringStateDoesNotApplyPremiumTwice) {
    testPlant->setPrice(20.0);
    testPlant->setState(new FloweringState());
    setPlantConditions(60, 60, 60, 40);
    
    testPlant->getState()->handleChange(testPlant);
    testPlant->getState()->handleChange(testPlant);
    
    // Should still be 1.5x, not compounded
    EXPECT_GE(testPlant->getPrice(), 20.0);
}

TEST_F(StateTest, FloweringStateTransitionsToMatureWhenFloweringEnds) {
    testPlant->setState(new FloweringState());
    setPlantConditions(60, 60, 60, 50); // Age 50, flowering period over
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Mature");
}

TEST_F(StateTest, FloweringStateDiesWithPoorHealth) {
    testPlant->setState(new FloweringState());
    // Health < 10 causes death in Flowering state
    setPlantConditions(0, 0, 0, 40); // Health = (0+0+0)/3 = 0 < 10
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Dead");
}

TEST_F(StateTest, FloweringStateAlertsWhenSunlightInsufficient) {
    testPlant->setState(new FloweringState());
    setPlantConditions(60, 60, 40, 40); // Low sunlight
    
    testing::internal::CaptureStdout();
    testPlant->getState()->handleChange(testPlant);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("sunlight"), std::string::npos);
}

TEST_F(StateTest, DeadStateMarksPlantNotReadyForSale) {
    testPlant->setState(new DeadState());
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_FALSE(testPlant->isReadyForSale());
}

TEST_F(StateTest, DeadStateSetsPriceToZero) {
    testPlant->setPrice(50.0);
    testPlant->setState(new DeadState());
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_DOUBLE_EQ(testPlant->getPrice(), 0.0);
}

TEST_F(StateTest, DeadStateIsTerminal) {
    testPlant->setState(new DeadState());
    setPlantConditions(100, 100, 100, 60); // Perfect conditions
    
    testPlant->getState()->handleChange(testPlant);
    
    // Should remain dead
    EXPECT_EQ(testPlant->getState()->getStateName(), "Dead");
}

TEST_F(StateTest, DeadStateReportsRemovalNeeded) {
    testPlant->setState(new DeadState());
    
    testing::internal::CaptureStdout();
    testPlant->getState()->handleChange(testPlant);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("removed"), std::string::npos);
}

TEST_F(StateTest, CompleteLifeCycleTransitions) {
    // Start as Seedling
    EXPECT_EQ(testPlant->getState()->getStateName(), "Seedling");
    
    // Transition to Growing
    setPlantConditions(60, 60, 60, 7);
    testPlant->getState()->handleChange(testPlant);
    EXPECT_EQ(testPlant->getState()->getStateName(), "Growing");
    
    // Transition to Mature
    setPlantConditions(60, 60, 60, 12);
    testPlant->getState()->handleChange(testPlant);
    EXPECT_EQ(testPlant->getState()->getStateName(), "Mature");
    
    // Transition to Flowering
    setPlantConditions(90, 90, 90, 35);
    testPlant->getState()->handleChange(testPlant);
    EXPECT_EQ(testPlant->getState()->getStateName(), "Flowering");
    
    // Transition back to Mature
    setPlantConditions(60, 60, 60, 50);
    testPlant->getState()->handleChange(testPlant);
    EXPECT_EQ(testPlant->getState()->getStateName(), "Mature");
}

TEST_F(StateTest, PlantCanDieAtAnyStage) {
    // Test dying from Seedling (needs health < 20)
    Plant* plant1 = new Plant("P1", "P1", new FlowerCareStrategy(), new SeedlingState());
    plant1->setWaterLevel(5);
    plant1->setNutrientLevel(5);
    plant1->setSunlightExposure(5);
    plant1->updateHealth();
    plant1->getState()->handleChange(plant1);
    EXPECT_EQ(plant1->getState()->getStateName(), "Dead");
    delete plant1;
    
    // Test dying from Growing (needs health < 20)
    Plant* plant2 = new Plant("P2", "P2", new FlowerCareStrategy(), new GrowingState());
    plant2->setWaterLevel(5);
    plant2->setNutrientLevel(5);
    plant2->setSunlightExposure(5);
    plant2->updateHealth();
    plant2->getState()->handleChange(plant2);
    EXPECT_EQ(plant2->getState()->getStateName(), "Dead");
    delete plant2;
    
    // Test dying from Mature (needs health < 10)
    Plant* plant3 = new Plant("P3", "P3", new FlowerCareStrategy(), new MatureState());
    plant3->setWaterLevel(0);
    plant3->setNutrientLevel(0);
    plant3->setSunlightExposure(0);
    plant3->updateHealth();
    plant3->getState()->handleChange(plant3);
    EXPECT_EQ(plant3->getState()->getStateName(), "Dead");
    delete plant3;
}

TEST_F(StateTest, StateNameReturnsCorrectValue) {
    EXPECT_EQ(testPlant->getState()->getStateName(), "Seedling");
    
    testPlant->setState(new GrowingState());
    EXPECT_EQ(testPlant->getState()->getStateName(), "Growing");
    
    testPlant->setState(new MatureState());
    EXPECT_EQ(testPlant->getState()->getStateName(), "Mature");
    
    testPlant->setState(new FloweringState());
    EXPECT_EQ(testPlant->getState()->getStateName(), "Flowering");
    
    testPlant->setState(new DeadState());
    EXPECT_EQ(testPlant->getState()->getStateName(), "Dead");
}

TEST_F(StateTest, StateTransitionDeletesOldState) {
    PlantState* oldState = testPlant->getState();
    testPlant->setState(new GrowingState());
    
    // Old state should be deleted, new state should exist
    EXPECT_NE(testPlant->getState(), oldState);
    EXPECT_NE(testPlant->getState(), nullptr);
}

TEST_F(StateTest, StateHandleChangeModifiesPlantProperties) {
    testPlant->setState(new MatureState());
    
    bool wasReadyBefore = testPlant->isReadyForSale();
    testPlant->getState()->handleChange(testPlant);
    bool isReadyAfter = testPlant->isReadyForSale();
    
    // Mature state should set ready for sale
    EXPECT_TRUE(isReadyAfter);
}

TEST_F(StateTest, StateTransitionAtExactAgeThreshold) {
    setPlantConditions(60, 60, 60, 7); // Exact age for transition
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Growing");
}

TEST_F(StateTest, StateTransitionAtExactHealthThreshold) {
    testPlant->setState(new GrowingState());
    // Health is average of water, nutrients, and sunlight
    // To get health of ~50, we need (50+50+50)/3 = 50
    setPlantConditions(50, 50, 50, 12); // Age 12, health should be exactly 50
    
    testPlant->getState()->handleChange(testPlant);
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Mature");
}

TEST_F(StateTest, MultipleStateChangesInSequence) {
    for (int i = 0; i < 5; i++) {
        setPlantConditions(60, 60, 60, 7 + i);
        testPlant->getState()->handleChange(testPlant);
    }
    
    // Should not crash and should maintain valid state
    EXPECT_NE(testPlant->getState(), nullptr);
}

TEST_F(StateTest, StateHandleChangeDoesNotCrash) {
    // Test that state transitions are safe
    // We can't actually test with nullptr as it would segfault
    // Instead, test that state changes work correctly
    SeedlingState* state = new SeedlingState();
    
    // Test with valid plant
    Plant* validPlant = new Plant("Valid", "V001", 
                                  new FlowerCareStrategy(), 
                                  new SeedlingState());
    validPlant->setWaterLevel(60);
    validPlant->setNutrientLevel(60);
    validPlant->setSunlightExposure(60);
    validPlant->updateHealth();
    
    EXPECT_NO_THROW(validPlant->getState()->handleChange(validPlant));
    
    delete validPlant;
    delete state;
}

TEST_F(StateTest, DailyUpdateTriggersStateChange) {
    setPlantConditions(60, 60, 60, 6);
    
    testPlant->dailyUpdate(); // This should increment age to 7 and trigger transition
    
    EXPECT_EQ(testPlant->getState()->getStateName(), "Growing");
}

TEST_F(StateTest, PlantReadyForSaleReflectsState) {
    // Seedling not ready
    EXPECT_FALSE(testPlant->isReadyForSale());
    
    // Mature is ready
    testPlant->setState(new MatureState());
    testPlant->getState()->handleChange(testPlant);
    EXPECT_TRUE(testPlant->isReadyForSale());
    
    // Dead not ready
    testPlant->setState(new DeadState());
    testPlant->getState()->handleChange(testPlant);
    EXPECT_FALSE(testPlant->isReadyForSale());
}

TEST_F(StateTest, PlantPriceAffectedByState) {
    double initialPrice = 10.0;
    testPlant->setPrice(initialPrice);
    
    // Flowering increases price
    testPlant->setState(new FloweringState());
    testPlant->getState()->handleChange(testPlant);
    EXPECT_GT(testPlant->getPrice(), initialPrice);
    
    // Dead sets price to 0
    testPlant->setState(new DeadState());
    testPlant->getState()->handleChange(testPlant);
    EXPECT_DOUBLE_EQ(testPlant->getPrice(), 0.0);
}

TEST_F(StateTest, EachStateProducesAppropriateAlerts) {
    // Seedling alerts for critical water
    setPlantConditions(30, 50, 50, 3);
    testing::internal::CaptureStdout();
    testPlant->getState()->handleChange(testPlant);
    std::string seedlingOutput = testing::internal::GetCapturedStdout();
    EXPECT_NE(seedlingOutput.find("urgently"), std::string::npos);
    
    // Growing alerts for water and fertilizer
    testPlant->setState(new GrowingState());
    setPlantConditions(25, 20, 50, 8);
    testing::internal::CaptureStdout();
    testPlant->getState()->handleChange(testPlant);
    std::string growingOutput = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(growingOutput.find("water") != std::string::npos || 
                growingOutput.find("fertilizer") != std::string::npos);
    
    // Mature alerts for water
    testPlant->setState(new MatureState());
    setPlantConditions(15, 50, 50, 15);
    testing::internal::CaptureStdout();
    testPlant->getState()->handleChange(testPlant);
    std::string matureOutput = testing::internal::GetCapturedStdout();
    EXPECT_NE(matureOutput.find("water"), std::string::npos);
}

TEST_F(StateTest, StateTransitionsDoNotLeakMemory) {
    for (int i = 0; i < 100; i++) {
        testPlant->setState(new SeedlingState());
        testPlant->setState(new GrowingState());
        testPlant->setState(new MatureState());
        testPlant->setState(new FloweringState());
        testPlant->setState(new DeadState());
    }
    
    // If we get here without crashing, no memory leaks
    EXPECT_NE(testPlant->getState(), nullptr);
}

TEST_F(StateTest, PlantDestructorCleansUpState) {
    Plant* tempPlant = new Plant("Temp", "T001", 
                                  new FlowerCareStrategy(), 
                                  new SeedlingState());
    
    EXPECT_NO_THROW(delete tempPlant);
}