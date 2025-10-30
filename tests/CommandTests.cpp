#include <gtest/gtest.h>
#include "include/Command.h"
#include "include/WaterPlantCommand.h"
#include "include/FertilizePlantCommand.h"
#include "include/AdjustSunlightCommand.h"
#include "include/CareScheduler.h"
#include "include/Plant.h"
#include "include/FlowerCareStrategy.h"
#include "include/SucculentCareStrategy.h"
#include "include/VegetableCareStrategy.h"
#include "include/OtherPlantCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/MatureState.h"

// ============ Test Fixture for Command Tests ============

class CommandTest : public ::testing::Test {
protected:
    CareScheduler* scheduler;
    Plant* flowerPlant;
    Plant* succulentPlant;
    Plant* vegetablePlant;
    
    void SetUp() override {
        // Create scheduler
        scheduler = new CareScheduler();
        
        // Create test plants with different strategies
        flowerPlant = new Plant("Rose", "R001", new FlowerCareStrategy(), new MatureState());
        flowerPlant->setWaterLevel(20);
        flowerPlant->setNutrientLevel(25);
        flowerPlant->setSunlightExposure(30);
        
        succulentPlant = new Plant("Cactus", "C001", new SucculentCareStrategy(), new MatureState());
        succulentPlant->setWaterLevel(15);
        succulentPlant->setNutrientLevel(20);
        succulentPlant->setSunlightExposure(25);
        
        vegetablePlant = new Plant("Tomato", "T001", new VegetableCareStrategy(), new MatureState());
        vegetablePlant->setWaterLevel(10);
        vegetablePlant->setNutrientLevel(15);
        vegetablePlant->setSunlightExposure(20);
    }
    
    void TearDown() override {
        delete scheduler;
        delete flowerPlant;
        delete succulentPlant;
        delete vegetablePlant;
    }
};

// ============ WaterPlantCommand Tests ============

TEST_F(CommandTest, WaterCommandExecutesOnFlower) {
    WaterPlantCommand* cmd = new WaterPlantCommand(flowerPlant);
    int initialWater = flowerPlant->getWaterLevel();
    
    cmd->execute();
    
    EXPECT_EQ(flowerPlant->getWaterLevel(), initialWater + 50); // FlowerCareStrategy adds 50
    delete cmd;
}

TEST_F(CommandTest, WaterCommandExecutesOnSucculent) {
    WaterPlantCommand* cmd = new WaterPlantCommand(succulentPlant);
    int initialWater = succulentPlant->getWaterLevel();
    
    cmd->execute();
    
    EXPECT_EQ(succulentPlant->getWaterLevel(), initialWater + 15); // SucculentCareStrategy adds 15
    delete cmd;
}

TEST_F(CommandTest, WaterCommandExecutesOnVegetable) {
    WaterPlantCommand* cmd = new WaterPlantCommand(vegetablePlant);
    int initialWater = vegetablePlant->getWaterLevel();
    
    cmd->execute();
    
    EXPECT_EQ(vegetablePlant->getWaterLevel(), 100); // VegetableCareStrategy adds 100, but capped at 100
    delete cmd;
}

TEST_F(CommandTest, WaterCommandWithNullPlant) {
    WaterPlantCommand* cmd = new WaterPlantCommand(nullptr);
    
    // Should not crash
    EXPECT_NO_THROW(cmd->execute());
    
    delete cmd;
}

TEST_F(CommandTest, WaterCommandWithNullStrategy) {
    Plant* plantNoStrategy = new Plant("Test", "T001", nullptr, new MatureState());
    WaterPlantCommand* cmd = new WaterPlantCommand(plantNoStrategy);
    
    // Should not crash
    EXPECT_NO_THROW(cmd->execute());
    
    delete cmd;
    delete plantNoStrategy;
}

// ============ FertilizePlantCommand Tests ============

TEST_F(CommandTest, FertilizeCommandExecutesOnFlower) {
    FertilizePlantCommand* cmd = new FertilizePlantCommand(flowerPlant);
    int initialNutrients = flowerPlant->getNutrientLevel();
    
    cmd->execute();
    
    EXPECT_EQ(flowerPlant->getNutrientLevel(), initialNutrients + 20); // FlowerCareStrategy adds 20
    delete cmd;
}

TEST_F(CommandTest, FertilizeCommandExecutesOnSucculent) {
    FertilizePlantCommand* cmd = new FertilizePlantCommand(succulentPlant);
    int initialNutrients = succulentPlant->getNutrientLevel();
    
    cmd->execute();
    
    EXPECT_EQ(succulentPlant->getNutrientLevel(), initialNutrients + 10); // SucculentCareStrategy adds 10
    delete cmd;
}

TEST_F(CommandTest, FertilizeCommandExecutesOnVegetable) {
    FertilizePlantCommand* cmd = new FertilizePlantCommand(vegetablePlant);
    int initialNutrients = vegetablePlant->getNutrientLevel();
    
    cmd->execute();
    
    EXPECT_EQ(vegetablePlant->getNutrientLevel(), initialNutrients + 25); // VegetableCareStrategy adds 25
    delete cmd;
}

TEST_F(CommandTest, FertilizeCommandWithNullPlant) {
    FertilizePlantCommand* cmd = new FertilizePlantCommand(nullptr);
    
    // Should not crash
    EXPECT_NO_THROW(cmd->execute());
    
    delete cmd;
}

// ============ AdjustSunlightCommand Tests ============

TEST_F(CommandTest, SunlightCommandExecutesOnFlower) {
    AdjustSunlightCommand* cmd = new AdjustSunlightCommand(flowerPlant);
    
    cmd->execute();
    
    EXPECT_EQ(flowerPlant->getSunlightExposure(), 70); // FlowerCareStrategy sets to 70
    delete cmd;
}

TEST_F(CommandTest, SunlightCommandExecutesOnSucculent) {
    AdjustSunlightCommand* cmd = new AdjustSunlightCommand(succulentPlant);
    
    cmd->execute();
    
    EXPECT_EQ(succulentPlant->getSunlightExposure(), 85); // SucculentCareStrategy sets to 85
    delete cmd;
}

TEST_F(CommandTest, SunlightCommandExecutesOnVegetable) {
    AdjustSunlightCommand* cmd = new AdjustSunlightCommand(vegetablePlant);
    
    cmd->execute();
    
    EXPECT_EQ(vegetablePlant->getSunlightExposure(), 75); // VegetableCareStrategy sets to 75
    delete cmd;
}

TEST_F(CommandTest, SunlightCommandWithNullPlant) {
    AdjustSunlightCommand* cmd = new AdjustSunlightCommand(nullptr);
    
    // Should not crash
    EXPECT_NO_THROW(cmd->execute());
    
    delete cmd;
}

// ============ CareScheduler Tests ============

TEST_F(CommandTest, SchedulerInitiallyEmpty) {
    EXPECT_TRUE(scheduler->empty());
}

TEST_F(CommandTest, SchedulerAddTaskNotEmpty) {
    scheduler->addTask(new WaterPlantCommand(flowerPlant));
    
    EXPECT_FALSE(scheduler->empty());
}

TEST_F(CommandTest, SchedulerAddNullTask) {
    scheduler->addTask(nullptr);
    
    EXPECT_TRUE(scheduler->empty());
}

TEST_F(CommandTest, SchedulerRunNextExecutesCommand) {
    scheduler->addTask(new WaterPlantCommand(flowerPlant));
    int initialWater = flowerPlant->getWaterLevel();
    
    scheduler->runNext();
    
    EXPECT_GT(flowerPlant->getWaterLevel(), initialWater);
    EXPECT_TRUE(scheduler->empty());
}

TEST_F(CommandTest, SchedulerRunNextOnEmptyQueue) {
    EXPECT_TRUE(scheduler->empty());
    
    // Should not crash
    EXPECT_NO_THROW(scheduler->runNext());
    
    EXPECT_TRUE(scheduler->empty());
}

TEST_F(CommandTest, SchedulerFIFOOrder) {
    // Add commands in specific order
    scheduler->addTask(new WaterPlantCommand(flowerPlant));
    scheduler->addTask(new FertilizePlantCommand(flowerPlant));
    scheduler->addTask(new AdjustSunlightCommand(flowerPlant));
    
    // Execute first command (water)
    int initialWater = flowerPlant->getWaterLevel();
    scheduler->runNext();
    EXPECT_GT(flowerPlant->getWaterLevel(), initialWater);
    
    // Execute second command (fertilize)
    int initialNutrients = flowerPlant->getNutrientLevel();
    scheduler->runNext();
    EXPECT_GT(flowerPlant->getNutrientLevel(), initialNutrients);
    
    // Execute third command (sunlight)
    scheduler->runNext();
    EXPECT_EQ(flowerPlant->getSunlightExposure(), 70);
    
    EXPECT_TRUE(scheduler->empty());
}

TEST_F(CommandTest, SchedulerRunAllExecutesAllCommands) {
    scheduler->addTask(new WaterPlantCommand(flowerPlant));
    scheduler->addTask(new FertilizePlantCommand(flowerPlant));
    scheduler->addTask(new AdjustSunlightCommand(flowerPlant));
    
    scheduler->runAll();
    
    EXPECT_TRUE(scheduler->empty());
    EXPECT_EQ(flowerPlant->getWaterLevel(), 70); // 20 + 50
    EXPECT_EQ(flowerPlant->getNutrientLevel(), 45); // 25 + 20
    EXPECT_EQ(flowerPlant->getSunlightExposure(), 70); // Set to 70
}

TEST_F(CommandTest, SchedulerRunAllOnEmptyQueue) {
    EXPECT_TRUE(scheduler->empty());
    
    // Should not crash
    EXPECT_NO_THROW(scheduler->runAll());
    
    EXPECT_TRUE(scheduler->empty());
}

TEST_F(CommandTest, SchedulerDeletesCommandsAfterExecution) {
    // Commands should be deleted by scheduler after execution
    scheduler->addTask(new WaterPlantCommand(flowerPlant));
    scheduler->runNext();
    
    EXPECT_TRUE(scheduler->empty());
    // Memory leak would be detected by valgrind/sanitizers
}

TEST_F(CommandTest, SchedulerCleansUpOnDestruction) {
    CareScheduler* tempScheduler = new CareScheduler();
    
    // Add commands but don't execute
    tempScheduler->addTask(new WaterPlantCommand(flowerPlant));
    tempScheduler->addTask(new FertilizePlantCommand(flowerPlant));
    
    // Destructor should clean up unexecuted commands
    delete tempScheduler;
    // Memory leak would be detected by valgrind/sanitizers
}

// ============ Command Integration Tests ============

TEST_F(CommandTest, MultipleCommandsOnSamePlant) {
    // Execute multiple commands on same plant
    scheduler->addTask(new WaterPlantCommand(flowerPlant));
    scheduler->addTask(new FertilizePlantCommand(flowerPlant));
    scheduler->addTask(new AdjustSunlightCommand(flowerPlant));
    
    scheduler->runAll();
    
    EXPECT_EQ(flowerPlant->getWaterLevel(), 70);
    EXPECT_EQ(flowerPlant->getNutrientLevel(), 45);
    EXPECT_EQ(flowerPlant->getSunlightExposure(), 70);
}

TEST_F(CommandTest, CommandsOnDifferentPlants) {
    scheduler->addTask(new WaterPlantCommand(flowerPlant));
    scheduler->addTask(new FertilizePlantCommand(succulentPlant));
    scheduler->addTask(new AdjustSunlightCommand(vegetablePlant));
    
    scheduler->runAll();
    
    EXPECT_EQ(flowerPlant->getWaterLevel(), 70); // 20 + 50
    EXPECT_EQ(succulentPlant->getNutrientLevel(), 30); // 20 + 10
    EXPECT_EQ(vegetablePlant->getSunlightExposure(), 75); // Set to 75
}

TEST_F(CommandTest, CommandsDelegateToCorrectStrategy) {
    // Test that commands correctly delegate to strategy methods
    Plant* otherPlant = new Plant("Generic", "G001", new OtherPlantCareStrategy(), new MatureState());
    otherPlant->setWaterLevel(50);
    otherPlant->setNutrientLevel(50);
    otherPlant->setSunlightExposure(50);
    
    WaterPlantCommand* waterCmd = new WaterPlantCommand(otherPlant);
    waterCmd->execute();
    EXPECT_EQ(otherPlant->getWaterLevel(), 70); // OtherPlantCareStrategy adds 20
    
    FertilizePlantCommand* fertCmd = new FertilizePlantCommand(otherPlant);
    fertCmd->execute();
    EXPECT_EQ(otherPlant->getNutrientLevel(), 60); // OtherPlantCareStrategy adds 10
    
    AdjustSunlightCommand* sunCmd = new AdjustSunlightCommand(otherPlant);
    sunCmd->execute();
    EXPECT_EQ(otherPlant->getSunlightExposure(), 60); // OtherPlantCareStrategy sets to 60
    
    delete waterCmd;
    delete fertCmd;
    delete sunCmd;
    delete otherPlant;
}

TEST_F(CommandTest, CommandsRespectPlantLimits) {
    // Test that commands respect the 0-100 limits
    flowerPlant->setWaterLevel(95);
    
    WaterPlantCommand* cmd = new WaterPlantCommand(flowerPlant);
    cmd->execute();
    
    EXPECT_EQ(flowerPlant->getWaterLevel(), 100); // Should cap at 100, not 145
    
    delete cmd;
}

TEST_F(CommandTest, LargeNumberOfCommands) {
    // Test scheduler can handle many commands
    for (int i = 0; i < 100; i++) {
        scheduler->addTask(new WaterPlantCommand(flowerPlant));
    }
    
    scheduler->runAll();
    
    EXPECT_TRUE(scheduler->empty());
    EXPECT_EQ(flowerPlant->getWaterLevel(), 100); // Should be capped at 100
}