#include <gtest/gtest.h>
#include <memory>
#include <string>

// Factory headers
#include "include/PlantFactory.h"
#include "include/RoseFactory.h"
#include "include/DaisyFactory.h"
#include "include/StrelitziaFactory.h"
#include "include/CactusFactory.h"
#include "include/AloeFactory.h"
#include "include/PotatoFactory.h"
#include "include/RadishFactory.h"
#include "include/MonsteraFactory.h"
#include "include/VenusFlyTrapFactory.h"

// Plant headers
#include "include/Plant.h"
#include "include/Flower.h"
#include "include/Rose.h"
#include "include/Daisy.h"
#include "include/Strelitzia.h"
#include "include/Succulent.h"
#include "include/Cactus.h"
#include "include/Aloe.h"
#include "include/Vegetable.h"
#include "include/Potato.h"
#include "include/Radish.h"
#include "include/OtherPlant.h"
#include "include/Monstera.h"
#include "include/VenusFlyTrap.h"

// Strategy and State headers
#include "include/CareStrategy.h"
#include "include/FlowerCareStrategy.h"
#include "include/SucculentCareStrategy.h"
#include "include/VegetableCareStrategy.h"
#include "include/OtherPlantCareStrategy.h"
#include "include/PlantState.h"
#include "include/SeedlingState.h"

// Observer and Command headers
#include "include/CareScheduler.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"

// ============ Base Factory Test Fixture ============

class FactoryTest : public ::testing::Test {
protected:
    CareScheduler* scheduler;
    
    void SetUp() override {
        scheduler = new CareScheduler();
    }
    
    void TearDown() override {
        delete scheduler;
    }
};

// ============ RoseFactory Tests ============

TEST_F(FactoryTest, RoseFactoryCreatesValidPlant) {
    RoseFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, RoseFactoryCreatesRoseType) {
    RoseFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    Rose* rose = dynamic_cast<Rose*>(plant);
    EXPECT_NE(rose, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, RoseFactoryAssignsCorrectStrategy) {
    RoseFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    CareStrategy* strategy = plant->getStrategy();
    ASSERT_NE(strategy, nullptr);
    
    FlowerCareStrategy* flowerStrategy = dynamic_cast<FlowerCareStrategy*>(strategy);
    EXPECT_NE(flowerStrategy, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, RoseFactoryAssignsSeedlingState) {
    RoseFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    PlantState* state = plant->getState();
    ASSERT_NE(state, nullptr);
    EXPECT_EQ(state->getStateName(), "Seedling");
    
    delete plant;
}

TEST_F(FactoryTest, RoseFactoryCreatesUniquePlants) {
    RoseFactory factory;
    Plant* plant1 = factory.buildPlant(scheduler);
    Plant* plant2 = factory.buildPlant(scheduler);
    
    EXPECT_NE(plant1->getID(), plant2->getID());
    
    delete plant1;
    delete plant2;
}

TEST_F(FactoryTest, RoseFactoryHandlesNullScheduler) {
    RoseFactory factory;
    Plant* plant = factory.buildPlant(nullptr);
    
    EXPECT_NE(plant, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, RoseFactoryIDIncrementsCorrectly) {
    RoseFactory factory;
    Plant* plant1 = factory.buildPlant(scheduler);
    Plant* plant2 = factory.buildPlant(scheduler);
    Plant* plant3 = factory.buildPlant(scheduler);
    
    EXPECT_NE(plant1->getID().find("ROSE_"), std::string::npos);
    EXPECT_NE(plant2->getID().find("ROSE_"), std::string::npos);
    EXPECT_NE(plant3->getID().find("ROSE_"), std::string::npos);
    
    delete plant1;
    delete plant2;
    delete plant3;
}

// ============ DaisyFactory Tests ============

TEST_F(FactoryTest, DaisyFactoryCreatesValidPlant) {
    DaisyFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, DaisyFactoryCreatesDaisyType) {
    DaisyFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    Daisy* daisy = dynamic_cast<Daisy*>(plant);
    EXPECT_NE(daisy, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, DaisyFactoryAssignsCorrectStrategy) {
    DaisyFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    FlowerCareStrategy* strategy = dynamic_cast<FlowerCareStrategy*>(plant->getStrategy());
    EXPECT_NE(strategy, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, DaisyFactoryCreatesUniquePlants) {
    DaisyFactory factory;
    Plant* plant1 = factory.buildPlant(scheduler);
    Plant* plant2 = factory.buildPlant(scheduler);
    
    EXPECT_NE(plant1->getID(), plant2->getID());
    
    delete plant1;
    delete plant2;
}

// ============ StrelitziaFactory Tests ============

TEST_F(FactoryTest, StrelitziaFactoryCreatesValidPlant) {
    StrelitziaFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, StrelitziaFactoryCreatesStrelitziaType) {
    StrelitziaFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    Strelitzia* strelitzia = dynamic_cast<Strelitzia*>(plant);
    EXPECT_NE(strelitzia, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, StrelitziaFactoryAssignsFlowerStrategy) {
    StrelitziaFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    FlowerCareStrategy* strategy = dynamic_cast<FlowerCareStrategy*>(plant->getStrategy());
    EXPECT_NE(strategy, nullptr);
    
    delete plant;
}

// ============ CactusFactory Tests ============

TEST_F(FactoryTest, CactusFactoryCreatesValidPlant) {
    CactusFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, CactusFactoryCreatesCactusType) {
    CactusFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    Cactus* cactus = dynamic_cast<Cactus*>(plant);
    EXPECT_NE(cactus, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, CactusFactoryAssignsSucculentStrategy) {
    CactusFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    SucculentCareStrategy* strategy = dynamic_cast<SucculentCareStrategy*>(plant->getStrategy());
    EXPECT_NE(strategy, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, CactusFactoryCreatesUniquePlants) {
    CactusFactory factory;
    Plant* plant1 = factory.buildPlant(scheduler);
    Plant* plant2 = factory.buildPlant(scheduler);
    
    EXPECT_NE(plant1->getID(), plant2->getID());
    
    delete plant1;
    delete plant2;
}

// ============ AloeFactory Tests ============

TEST_F(FactoryTest, AloeFactoryCreatesValidPlant) {
    AloeFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, AloeFactoryCreatesAloeType) {
    AloeFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    Aloe* aloe = dynamic_cast<Aloe*>(plant);
    EXPECT_NE(aloe, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, AloeFactoryAssignsSucculentStrategy) {
    AloeFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    SucculentCareStrategy* strategy = dynamic_cast<SucculentCareStrategy*>(plant->getStrategy());
    EXPECT_NE(strategy, nullptr);
    
    delete plant;
}

// ============ PotatoFactory Tests ============

TEST_F(FactoryTest, PotatoFactoryCreatesValidPlant) {
    PotatoFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, PotatoFactoryCreatesPotatoType) {
    PotatoFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    Potato* potato = dynamic_cast<Potato*>(plant);
    EXPECT_NE(potato, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, PotatoFactoryAssignsVegetableStrategy) {
    PotatoFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    VegetableCareStrategy* strategy = dynamic_cast<VegetableCareStrategy*>(plant->getStrategy());
    EXPECT_NE(strategy, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, PotatoFactoryCreatesUniquePlants) {
    PotatoFactory factory;
    Plant* plant1 = factory.buildPlant(scheduler);
    Plant* plant2 = factory.buildPlant(scheduler);
    
    EXPECT_NE(plant1->getID(), plant2->getID());
    
    delete plant1;
    delete plant2;
}

// ============ RadishFactory Tests ============

TEST_F(FactoryTest, RadishFactoryCreatesValidPlant) {
    RadishFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, RadishFactoryCreatesRadishType) {
    RadishFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    Radish* radish = dynamic_cast<Radish*>(plant);
    EXPECT_NE(radish, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, RadishFactoryAssignsVegetableStrategy) {
    RadishFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    VegetableCareStrategy* strategy = dynamic_cast<VegetableCareStrategy*>(plant->getStrategy());
    EXPECT_NE(strategy, nullptr);
    
    delete plant;
}

// ============ MonsteraFactory Tests ============

TEST_F(FactoryTest, MonsteraFactoryCreatesValidPlant) {
    MonsteraFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, MonsteraFactoryCreatesMonsteraType) {
    MonsteraFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    Monstera* monstera = dynamic_cast<Monstera*>(plant);
    EXPECT_NE(monstera, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, MonsteraFactoryAssignsOtherPlantStrategy) {
    MonsteraFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    OtherPlantCareStrategy* strategy = dynamic_cast<OtherPlantCareStrategy*>(plant->getStrategy());
    EXPECT_NE(strategy, nullptr);
    
    delete plant;
}

// ============ VenusFlyTrapFactory Tests ============

TEST_F(FactoryTest, VenusFlyTrapFactoryCreatesValidPlant) {
    VenusFlyTrapFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    ASSERT_NE(plant, nullptr);
    EXPECT_NE(plant->getID(), "");
    
    delete plant;
}

TEST_F(FactoryTest, VenusFlyTrapFactoryCreatesVenusFlyTrapType) {
    VenusFlyTrapFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    VenusFlyTrap* vft = dynamic_cast<VenusFlyTrap*>(plant);
    EXPECT_NE(vft, nullptr);
    
    delete plant;
}

TEST_F(FactoryTest, VenusFlyTrapFactoryAssignsOtherPlantStrategy) {
    VenusFlyTrapFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    OtherPlantCareStrategy* strategy = dynamic_cast<OtherPlantCareStrategy*>(plant->getStrategy());
    EXPECT_NE(strategy, nullptr);
    
    delete plant;
}

// ============ Observer Attachment Tests ============

TEST_F(FactoryTest, FactoryAttachesObserversWithScheduler) {
    RoseFactory factory;
    Plant* plant = factory.buildPlant(scheduler);
    
    // Trigger state change to verify observers are working
    plant->setWaterLevel(25);
    plant->notify();
    
    // If observers attached correctly, tasks should be in scheduler
    EXPECT_FALSE(scheduler->empty());
    
    delete plant;
}

TEST_F(FactoryTest, FactoryDoesNotAttachObserversWithoutScheduler) {
    RoseFactory factory;
    Plant* plant = factory.buildPlant(nullptr);
    
    // Should not crash
    plant->setWaterLevel(25);
    plant->notify();
    
    delete plant;
}

// ============ Multiple Factory Integration Tests ============

TEST_F(FactoryTest, DifferentFactoriesCreateDifferentTypes) {
    RoseFactory roseFactory;
    CactusFactory cactusFactory;
    PotatoFactory potatoFactory;
    
    Plant* rose = roseFactory.buildPlant(scheduler);
    Plant* cactus = cactusFactory.buildPlant(scheduler);
    Plant* potato = potatoFactory.buildPlant(scheduler);
    
    EXPECT_NE(dynamic_cast<Rose*>(rose), nullptr);
    EXPECT_NE(dynamic_cast<Cactus*>(cactus), nullptr);
    EXPECT_NE(dynamic_cast<Potato*>(potato), nullptr);
    
    delete rose;
    delete cactus;
    delete potato;
}

TEST_F(FactoryTest, DifferentFactoriesAssignDifferentStrategies) {
    RoseFactory roseFactory;
    CactusFactory cactusFactory;
    PotatoFactory potatoFactory;
    
    Plant* rose = roseFactory.buildPlant(scheduler);
    Plant* cactus = cactusFactory.buildPlant(scheduler);
    Plant* potato = potatoFactory.buildPlant(scheduler);
    
    EXPECT_NE(dynamic_cast<FlowerCareStrategy*>(rose->getStrategy()), nullptr);
    EXPECT_NE(dynamic_cast<SucculentCareStrategy*>(cactus->getStrategy()), nullptr);
    EXPECT_NE(dynamic_cast<VegetableCareStrategy*>(potato->getStrategy()), nullptr);
    
    delete rose;
    delete cactus;
    delete potato;
}

// ============ Consistency Tests ============

TEST_F(FactoryTest, AllFactoriesAssignSeedlingState) {
    RoseFactory roseFactory;
    CactusFactory cactusFactory;
    PotatoFactory potatoFactory;
    MonsteraFactory monsteraFactory;
    
    Plant* rose = roseFactory.buildPlant(scheduler);
    Plant* cactus = cactusFactory.buildPlant(scheduler);
    Plant* potato = potatoFactory.buildPlant(scheduler);
    Plant* monstera = monsteraFactory.buildPlant(scheduler);
    
    EXPECT_EQ(rose->getState()->getStateName(), "Seedling");
    EXPECT_EQ(cactus->getState()->getStateName(), "Seedling");
    EXPECT_EQ(potato->getState()->getStateName(), "Seedling");
    EXPECT_EQ(monstera->getState()->getStateName(), "Seedling");
    
    delete rose;
    delete cactus;
    delete potato;
    delete monstera;
}

TEST_F(FactoryTest, AllFactoriesProduceValidIDs) {
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    
    Plant* rose = roseFactory.buildPlant(scheduler);
    Plant* daisy = daisyFactory.buildPlant(scheduler);
    Plant* cactus = cactusFactory.buildPlant(scheduler);
    
    EXPECT_FALSE(rose->getID().empty());
    EXPECT_FALSE(daisy->getID().empty());
    EXPECT_FALSE(cactus->getID().empty());
    
    EXPECT_NE(rose->getID(), daisy->getID());
    EXPECT_NE(daisy->getID(), cactus->getID());
    EXPECT_NE(rose->getID(), cactus->getID());
    
    delete rose;
    delete daisy;
    delete cactus;
}

// ============ Memory Management Tests ============

TEST_F(FactoryTest, FactoryPlantsCanBeDeletedSafely) {
    RoseFactory factory;
    
    for (int i = 0; i < 10; i++) {
        Plant* plant = factory.buildPlant(scheduler);
        EXPECT_NO_THROW(delete plant);
    }
}

TEST_F(FactoryTest, MultipleFactoriesWithSameScheduler) {
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    
    Plant* rose = roseFactory.buildPlant(scheduler);
    Plant* daisy = daisyFactory.buildPlant(scheduler);
    Plant* cactus = cactusFactory.buildPlant(scheduler);
    
    // All plants share same scheduler
    rose->setWaterLevel(25);
    rose->notify();
    
    daisy->setNutrientLevel(25);
    daisy->notify();
    
    cactus->setSunlightExposure(35);
    cactus->notify();
    
    // Scheduler should have tasks from all plants
    EXPECT_FALSE(scheduler->empty());
    
    delete rose;
    delete daisy;
    delete cactus;
}