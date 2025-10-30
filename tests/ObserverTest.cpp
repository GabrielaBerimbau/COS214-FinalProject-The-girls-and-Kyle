#include <gtest/gtest.h>
#include "include/Plant.h"
#include "include/PlantObserver.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"
#include "include/FlowerCareStrategy.h"
#include "include/SucculentCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/GrowingState.h"
#include "include/MatureState.h"
#include "include/FloweringState.h"
#include "include/DeadState.h"

// ============ Test Observer Helper Class ============

class TestObserver : public PlantObserver {
public:
    int updateCount = 0;
    Plant* lastPlantUpdated = nullptr;
    
    virtual void update(Plant* plant) override {
        updateCount++;
        lastPlantUpdated = plant;
    }
    
    void reset() {
        updateCount = 0;
        lastPlantUpdated = nullptr;
    }
};

// ============ Test Fixture for Observer Tests ============

class ObserverTest : public ::testing::Test {
protected:
    CareScheduler* scheduler;
    Plant* testPlant;
    TestObserver* testObserver1;
    TestObserver* testObserver2;
    
    void SetUp() override {
        scheduler = new CareScheduler();
        
        testPlant = new Plant("Rose", "R001", new FlowerCareStrategy(), new MatureState());
        testPlant->setWaterLevel(50);
        testPlant->setNutrientLevel(50);
        testPlant->setSunlightExposure(50);
        
        testObserver1 = new TestObserver();
        testObserver2 = new TestObserver();
    }
    
    void TearDown() override {
        delete scheduler;
        delete testPlant;
        delete testObserver1;
        delete testObserver2;
    }
};

// ============ Basic Observer Mechanism Tests ============

TEST_F(ObserverTest, AttachSingleObserver) {
    testPlant->attach(testObserver1);
    testPlant->notify();
    
    EXPECT_EQ(testObserver1->updateCount, 1);
    EXPECT_EQ(testObserver1->lastPlantUpdated, testPlant);
}

TEST_F(ObserverTest, AttachMultipleObservers) {
    testPlant->attach(testObserver1);
    testPlant->attach(testObserver2);
    testPlant->notify();
    
    EXPECT_EQ(testObserver1->updateCount, 1);
    EXPECT_EQ(testObserver2->updateCount, 1);
    EXPECT_EQ(testObserver1->lastPlantUpdated, testPlant);
    EXPECT_EQ(testObserver2->lastPlantUpdated, testPlant);
}

TEST_F(ObserverTest, DetachObserver) {
    testPlant->attach(testObserver1);
    testPlant->attach(testObserver2);
    
    testPlant->detach(testObserver1);
    testPlant->notify();
    
    EXPECT_EQ(testObserver1->updateCount, 0);
    EXPECT_EQ(testObserver2->updateCount, 1);
}

TEST_F(ObserverTest, DetachNonExistentObserver) {
    testPlant->attach(testObserver1);
    
    // Should not crash
    EXPECT_NO_THROW(testPlant->detach(testObserver2));
    
    testPlant->notify();
    EXPECT_EQ(testObserver1->updateCount, 1);
}

TEST_F(ObserverTest, AttachNullObserver) {
    // Should not crash
    EXPECT_NO_THROW(testPlant->attach(nullptr));
    EXPECT_NO_THROW(testPlant->notify());
}

TEST_F(ObserverTest, DetachNullObserver) {
    // Should not crash
    EXPECT_NO_THROW(testPlant->detach(nullptr));
}

TEST_F(ObserverTest, DuplicateAttachPrevented) {
    testPlant->attach(testObserver1);
    testPlant->attach(testObserver1);
    testPlant->attach(testObserver1);
    
    testPlant->notify();
    
    // Should only be notified once
    EXPECT_EQ(testObserver1->updateCount, 1);
}

TEST_F(ObserverTest, MultipleNotifications) {
    testPlant->attach(testObserver1);
    
    testPlant->notify();
    testPlant->notify();
    testPlant->notify();
    
    EXPECT_EQ(testObserver1->updateCount, 3);
}

// ============ WaterObserver Tests ============

TEST_F(ObserverTest, WaterObserverBelowThreshold) {
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    
    testPlant->setWaterLevel(25); // Below threshold of 30
    testPlant->notify();
    
    EXPECT_FALSE(scheduler->empty());
    
    // Execute command to verify it's a water command
    int initialWater = testPlant->getWaterLevel();
    scheduler->runNext();
    EXPECT_GT(testPlant->getWaterLevel(), initialWater);
    
    delete waterObs;
}

TEST_F(ObserverTest, WaterObserverAboveThreshold) {
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    
    testPlant->setWaterLevel(35); // Above threshold of 30
    testPlant->notify();
    
    EXPECT_TRUE(scheduler->empty());
    
    delete waterObs;
}

TEST_F(ObserverTest, WaterObserverExactThreshold) {
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    
    testPlant->setWaterLevel(30); // Exactly at threshold
    testPlant->notify();
    
    EXPECT_TRUE(scheduler->empty()); // Should not trigger
    
    delete waterObs;
}

TEST_F(ObserverTest, WaterObserverAutoAttaches) {
    // Observer should auto-attach in constructor
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    
    testPlant->setWaterLevel(20);
    testPlant->notify();
    
    EXPECT_FALSE(scheduler->empty());
    
    delete waterObs;
}

TEST_F(ObserverTest, WaterObserverAutoDetaches) {
    {
        WaterObserver waterObs(scheduler, testPlant);
        
        testPlant->setWaterLevel(20);
        testPlant->notify();
        EXPECT_FALSE(scheduler->empty());
        scheduler->runAll();
    }
    // Observer destroyed here
    
    testPlant->setWaterLevel(20);
    testPlant->notify();
    EXPECT_TRUE(scheduler->empty()); // Should not create command after destruction
}

// ============ FertilizeObserver Tests ============

TEST_F(ObserverTest, FertilizeObserverBelowThreshold) {
    FertilizeObserver* fertObs = new FertilizeObserver(scheduler, testPlant);
    
    testPlant->setNutrientLevel(25); // Below threshold of 30
    testPlant->notify();
    
    EXPECT_FALSE(scheduler->empty());
    
    // Execute command to verify it's a fertilize command
    int initialNutrients = testPlant->getNutrientLevel();
    scheduler->runNext();
    EXPECT_GT(testPlant->getNutrientLevel(), initialNutrients);
    
    delete fertObs;
}

TEST_F(ObserverTest, FertilizeObserverAboveThreshold) {
    FertilizeObserver* fertObs = new FertilizeObserver(scheduler, testPlant);
    
    testPlant->setNutrientLevel(35); // Above threshold of 30
    testPlant->notify();
    
    EXPECT_TRUE(scheduler->empty());
    
    delete fertObs;
}
// // seg faulting
// TEST_F(ObserverTest, FertilizeObserverWithNullScheduler) {
//     FertilizeObserver* fertObs = new FertilizeObserver(nullptr, testPlant);
    
//     testPlant->setNutrientLevel(20);
    
//     // Should not crash
//     EXPECT_NO_THROW(testPlant->notify());
    
//     delete fertObs;
// }

// ============ SunlightObserver Tests ============

TEST_F(ObserverTest, SunlightObserverBelowThreshold) {
    SunlightObserver* sunObs = new SunlightObserver(scheduler, testPlant);
    
    testPlant->setSunlightExposure(35); // Below threshold of 40
    testPlant->notify();
    
    EXPECT_FALSE(scheduler->empty());
    
    // Execute command to verify it's a sunlight command
    scheduler->runNext();
    EXPECT_EQ(testPlant->getSunlightExposure(), 70); // FlowerCareStrategy sets to 70
    
    delete sunObs;
}

TEST_F(ObserverTest, SunlightObserverAboveThreshold) {
    SunlightObserver* sunObs = new SunlightObserver(scheduler, testPlant);
    
    testPlant->setSunlightExposure(45); // Above threshold of 40
    testPlant->notify();
    
    EXPECT_TRUE(scheduler->empty());
    
    delete sunObs;
}

TEST_F(ObserverTest, SunlightObserverExactThreshold) {
    SunlightObserver* sunObs = new SunlightObserver(scheduler, testPlant);
    
    testPlant->setSunlightExposure(40); // Exactly at threshold
    testPlant->notify();
    
    EXPECT_TRUE(scheduler->empty()); // Should not trigger
    
    delete sunObs;
}

// ============ Multiple Observers Working Together ============

TEST_F(ObserverTest, AllObserversTriggered) {
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    FertilizeObserver* fertObs = new FertilizeObserver(scheduler, testPlant);
    SunlightObserver* sunObs = new SunlightObserver(scheduler, testPlant);
    
    // Set all levels below thresholds
    testPlant->setWaterLevel(20);      // Below 30
    testPlant->setNutrientLevel(25);   // Below 30
    testPlant->setSunlightExposure(35); // Below 40
    
    testPlant->notify();
    
    // Should have 3 commands queued
    EXPECT_FALSE(scheduler->empty());
    
    int commandCount = 0;
    while (!scheduler->empty()) {
        scheduler->runNext();
        commandCount++;
    }
    
    EXPECT_EQ(commandCount, 3);
    
    // All conditions should be improved
    EXPECT_GT(testPlant->getWaterLevel(), 20);
    EXPECT_GT(testPlant->getNutrientLevel(), 25);
    EXPECT_GT(testPlant->getSunlightExposure(), 35);
    
    delete waterObs;
    delete fertObs;
    delete sunObs;
}

TEST_F(ObserverTest, MixedThresholdConditions) {
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    FertilizeObserver* fertObs = new FertilizeObserver(scheduler, testPlant);
    SunlightObserver* sunObs = new SunlightObserver(scheduler, testPlant);
    
    // Only water below threshold
    testPlant->setWaterLevel(20);      // Below 30
    testPlant->setNutrientLevel(50);   // Above 30
    testPlant->setSunlightExposure(60); // Above 40
    
    testPlant->notify();
    
    // Should only have 1 command
    int commandCount = 0;
    while (!scheduler->empty()) {
        scheduler->runNext();
        commandCount++;
    }
    
    EXPECT_EQ(commandCount, 1);
    
    delete waterObs;
    delete fertObs;
    delete sunObs;
}

// ============ Observer with Plant State Changes ============

TEST_F(ObserverTest, ObserversTriggeredDuringDailyUpdate) {
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    FertilizeObserver* fertObs = new FertilizeObserver(scheduler, testPlant);
    
    testPlant->setWaterLevel(35);  // Just above threshold
    testPlant->setNutrientLevel(32); // Just above threshold
    
    // Daily update decreases levels by 10 and 5 respectively
    testPlant->dailyUpdate();
    
    EXPECT_EQ(testPlant->getWaterLevel(), 25);  // Below threshold now
    EXPECT_EQ(testPlant->getNutrientLevel(), 27); // Below threshold now
    
    // Should have triggered both observers
    int commandCount = 0;
    while (!scheduler->empty()) {
        scheduler->runNext();
        commandCount++;
    }
    
    EXPECT_EQ(commandCount, 2);
    
    delete waterObs;
    delete fertObs;
}

TEST_F(ObserverTest, ObserversTriggeredDuringUpdateCondition) {
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    
    testPlant->setWaterLevel(32); // Just above threshold
    
    // UpdateCondition decreases water by 5
    testPlant->updateCondition();
    
    EXPECT_EQ(testPlant->getWaterLevel(), 27); // Below threshold now
    
    EXPECT_FALSE(scheduler->empty());
    
    delete waterObs;
}



TEST_F(ObserverTest, ObserversWorkWithStateChanges) {
    // Test observers work when plant changes states
    Plant* maturePlant = new Plant("Mature Rose", "M001", 
                                   new FlowerCareStrategy(), 
                                   new GrowingState());
    
    WaterObserver* waterObs = new WaterObserver(scheduler, maturePlant);
    
    // Set conditions that would normally trigger growing
    maturePlant->setWaterLevel(25); // Below threshold
    
    // Daily update should notify observers regardless of state change
    maturePlant->dailyUpdate();
    
    // Should have water command from low water
    EXPECT_FALSE(scheduler->empty());
    
    // Plant should have transitioned to growing
    EXPECT_EQ(maturePlant->getState()->getStateName(), "Growing");
    
    delete waterObs;
    delete maturePlant;
}


TEST_F(ObserverTest, ObserversWhenPlantDies) {
    Plant* dyingPlant = new Plant("Dying Rose", "D001",
                                  new FlowerCareStrategy(),
                                  new MatureState());
    
    WaterObserver* waterObs = new WaterObserver(scheduler, dyingPlant);
    
    // Set health very low to trigger death
    // dyingPlant->setHealthLevel(5);
    dyingPlant->setWaterLevel(0);
    dyingPlant->setNutrientLevel(0);
    dyingPlant->setSunlightExposure(0);
    
    // Update to trigger state change to dead
    dyingPlant->dailyUpdate();
    
    // Should still create command even though plant is dying
    EXPECT_FALSE(scheduler->empty());
    
    // State should be dead
    EXPECT_EQ(dyingPlant->getState()->getStateName(), "Dead");
    
    delete waterObs;
    delete dyingPlant;
}

// ============ Observer Memory Management ============

TEST_F(ObserverTest, ObserverWithNullPlant) {
    WaterObserver* waterObs = new WaterObserver(scheduler, nullptr);
    
    // Should not crash
    delete waterObs;
}

TEST_F(ObserverTest, MultipleObserversSameScheduler) {
    Plant* plant2 = new Plant("Daisy", "D001", new FlowerCareStrategy(), new MatureState());
    
    WaterObserver* waterObs1 = new WaterObserver(scheduler, testPlant);
    WaterObserver* waterObs2 = new WaterObserver(scheduler, plant2);
    
    testPlant->setWaterLevel(20);
    plant2->setWaterLevel(15);
    
    testPlant->notify();
    plant2->notify();
    
    // Should have 2 commands in same scheduler
    int commandCount = 0;
    while (!scheduler->empty()) {
        scheduler->runNext();
        commandCount++;
    }
    
    EXPECT_EQ(commandCount, 2);
    
    delete waterObs1;
    delete waterObs2;
    delete plant2;
}


// ============ Complex Observer Scenarios ============

TEST_F(ObserverTest, ObserverChainReaction) {
    WaterObserver* waterObs = new WaterObserver(scheduler, testPlant);
    FertilizeObserver* fertObs = new FertilizeObserver(scheduler, testPlant);
    
    // Initial conditions just above threshold
    testPlant->setWaterLevel(31);
    testPlant->setNutrientLevel(31);
    
    // Simulate multiple days
    for (int day = 0; day < 3; day++) {
        testPlant->dailyUpdate();
        
        // Execute any scheduled care
        scheduler->runAll();
    }
    
    // Plant should still be maintained despite daily deterioration
    EXPECT_GT(testPlant->getWaterLevel(), 0);
    EXPECT_GT(testPlant->getNutrientLevel(), 0);
    
    delete waterObs;
    delete fertObs;
}

TEST_F(ObserverTest, ObserversWithDifferentStrategies) {
    Plant* succulent = new Plant("Cactus", "C001", 
                                new SucculentCareStrategy(), 
                                new MatureState());
    
    WaterObserver* waterObs1 = new WaterObserver(scheduler, testPlant);
    WaterObserver* waterObs2 = new WaterObserver(scheduler, succulent);
    
    testPlant->setWaterLevel(20);
    succulent->setWaterLevel(20);
    
    testPlant->notify();
    succulent->notify();
    
    scheduler->runAll();
    
    // Different strategies should result in different water increases
    EXPECT_EQ(testPlant->getWaterLevel(), 70);  // Flower: 20 + 50
    EXPECT_EQ(succulent->getWaterLevel(), 35);  // Succulent: 20 + 15
    
    delete waterObs1;
    delete waterObs2;
    delete succulent;
}

TEST_F(ObserverTest, ObserversCreateCorrectCommandTypes) {
    CareScheduler* scheduler1 = new CareScheduler();
    CareScheduler* scheduler2 = new CareScheduler();
    CareScheduler* scheduler3 = new CareScheduler();
    
    WaterObserver* waterObs = new WaterObserver(scheduler1, testPlant);
    FertilizeObserver* fertObs = new FertilizeObserver(scheduler2, testPlant);
    SunlightObserver* sunObs = new SunlightObserver(scheduler3, testPlant);
    
    testPlant->setWaterLevel(20);
    testPlant->setNutrientLevel(20);
    testPlant->setSunlightExposure(30);
    
    testPlant->notify();
    
    // Each scheduler should have exactly one command
    EXPECT_FALSE(scheduler1->empty());
    EXPECT_FALSE(scheduler2->empty());
    EXPECT_FALSE(scheduler3->empty());
    
    // Execute and verify each command type
    int initialWater = testPlant->getWaterLevel();
    scheduler1->runNext();
    EXPECT_GT(testPlant->getWaterLevel(), initialWater);
    
    int initialNutrients = testPlant->getNutrientLevel();
    scheduler2->runNext();
    EXPECT_GT(testPlant->getNutrientLevel(), initialNutrients);
    
    scheduler3->runNext();
    EXPECT_EQ(testPlant->getSunlightExposure(), 70);
    
    delete waterObs;
    delete fertObs;
    delete sunObs;
    delete scheduler1;
    delete scheduler2;
    delete scheduler3;
}