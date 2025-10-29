#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

// Include the actual header files based on the implementation
#include "include/DaisyFactory.h"
#include "include/Daisy.h"
#include "include/Plant.h"
#include "include/CareScheduler.h"
#include "include/FlowerCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"


// ============ DaisyFactory Tests ============

class DaisyFactoryTest : public ::testing::Test {
protected:
    DaisyFactory* factory;
    CareScheduler* scheduler;
    
    void SetUp() override {
        factory = new DaisyFactory();
        scheduler = new CareScheduler();
    }
    
    void TearDown() override {
        delete factory;
        delete scheduler;
    }
};

TEST_F(DaisyFactoryTest, CreatesValidPlant) {
    Plant* plant = factory->buildPlant(scheduler);
    EXPECT_NE(plant, nullptr);
    delete plant;
}

TEST_F(DaisyFactoryTest, HandlesNullScheduler) {
    Plant* plant = factory->buildPlant(nullptr);
    EXPECT_NE(plant, nullptr);
    delete plant;
}

TEST_F(DaisyFactoryTest, CreatesDaisyType) {
    Plant* plant = factory->buildPlant(scheduler);
    Daisy* daisy = dynamic_cast<Daisy*>(plant);
    EXPECT_NE(daisy, nullptr);
    delete plant;
}

TEST_F(DaisyFactoryTest, CreatesUniquePlants) {
    Plant* plant1 = factory->buildPlant(scheduler);
    Plant* plant2 = factory->buildPlant(scheduler);
    
    EXPECT_NE(plant1, nullptr);
    EXPECT_NE(plant2, nullptr);
    EXPECT_NE(plant1, plant2);
    
    delete plant1;
    delete plant2;
}