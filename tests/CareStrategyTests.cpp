/**
 * @file CareStrategyTests.cpp
 * @author Rachel Clifford
 * @date October 30, 2025
 * @brief Google Test unit tests for CareStrategy classes only
 */

#include <gtest/gtest.h>
#include "include/CareStrategy.h"
#include "include/FlowerCareStrategy.h"
#include "include/SucculentCareStrategy.h"
#include "include/VegetableCareStrategy.h"
#include "include/OtherPlantCareStrategy.h"
#include "include/Plant.h"

// ==================== FlowerCareStrategy Tests ====================

TEST(FlowerCareStrategyTest, WaterAdds50Units) {
    FlowerCareStrategy strategy;
    Plant plant("Test", "001", nullptr, nullptr);
    plant.setWaterLevel(50);
    strategy.water(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 100) << "Water should be 50 + 50 = 100";
}

TEST(FlowerCareStrategyTest, FertilizeAdds20Units) {
    FlowerCareStrategy strategy;
    Plant plant("Test", "002", nullptr, nullptr);
    plant.setNutrientLevel(50);
    strategy.fertilize(&plant);
    EXPECT_EQ(plant.getNutrientLevel(), 70) << "Nutrients should be 50 + 20 = 70";
}

TEST(FlowerCareStrategyTest, AdjustSunlightSetsTo70) {
    FlowerCareStrategy strategy;
    Plant plant("Test", "003", nullptr, nullptr);
    strategy.adjustSunlight(&plant);
    EXPECT_EQ(plant.getSunlightExposure(), 70);
}

TEST(FlowerCareStrategyTest, PerformCareCallsAllMethods) {
    FlowerCareStrategy strategy;
    Plant plant("Test", "004", nullptr, nullptr);
    plant.setWaterLevel(40);
    plant.setNutrientLevel(40);
    strategy.performCare(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 90);
    EXPECT_EQ(plant.getNutrientLevel(), 60);
    EXPECT_EQ(plant.getSunlightExposure(), 70);
}

// ==================== SucculentCareStrategy Tests ====================

TEST(SucculentCareStrategyTest, WaterAdds15Units) {
    SucculentCareStrategy strategy;
    Plant plant("Test", "005", nullptr, nullptr);
    plant.setWaterLevel(50);
    strategy.water(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 65);
}

TEST(SucculentCareStrategyTest, FertilizeAdds10Units) {
    SucculentCareStrategy strategy;
    Plant plant("Test", "006", nullptr, nullptr);
    plant.setNutrientLevel(50);
    strategy.fertilize(&plant);
    EXPECT_EQ(plant.getNutrientLevel(), 60);
}

TEST(SucculentCareStrategyTest, AdjustSunlightSetsTo85) {
    SucculentCareStrategy strategy;
    Plant plant("Test", "007", nullptr, nullptr);
    strategy.adjustSunlight(&plant);
    EXPECT_EQ(plant.getSunlightExposure(), 85);
}

TEST(SucculentCareStrategyTest, PerformCareCallsAllMethods) {
    SucculentCareStrategy strategy;
    Plant plant("Test", "008", nullptr, nullptr);
    plant.setWaterLevel(40);
    plant.setNutrientLevel(40);
    strategy.performCare(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 55);
    EXPECT_EQ(plant.getNutrientLevel(), 50);
    EXPECT_EQ(plant.getSunlightExposure(), 85);
}

// ==================== VegetableCareStrategy Tests ====================

TEST(VegetableCareStrategyTest, WaterAdds100Units) {
    VegetableCareStrategy strategy;
    Plant plant("Test", "009", nullptr, nullptr);
    plant.setWaterLevel(50);
    strategy.water(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 100);
}

TEST(VegetableCareStrategyTest, FertilizeAdds25Units) {
    VegetableCareStrategy strategy;
    Plant plant("Test", "010", nullptr, nullptr);
    plant.setNutrientLevel(50);
    strategy.fertilize(&plant);
    EXPECT_EQ(plant.getNutrientLevel(), 75);
}

TEST(VegetableCareStrategyTest, AdjustSunlightSetsTo75) {
    VegetableCareStrategy strategy;
    Plant plant("Test", "011", nullptr, nullptr);
    strategy.adjustSunlight(&plant);
    EXPECT_EQ(plant.getSunlightExposure(), 75);
}

TEST(VegetableCareStrategyTest, PerformCareCallsAllMethods) {
    VegetableCareStrategy strategy;
    Plant plant("Test", "012", nullptr, nullptr);
    plant.setWaterLevel(40);
    plant.setNutrientLevel(40);
    strategy.performCare(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 100);
    EXPECT_EQ(plant.getNutrientLevel(), 65);
    EXPECT_EQ(plant.getSunlightExposure(), 75);
}

// ==================== OtherPlantCareStrategy Tests ====================

TEST(OtherPlantCareStrategyTest, WaterAdds20Units) {
    OtherPlantCareStrategy strategy;
    Plant plant("Test", "013", nullptr, nullptr);
    plant.setWaterLevel(50);
    strategy.water(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 70);
}

TEST(OtherPlantCareStrategyTest, FertilizeAdds10Units) {
    OtherPlantCareStrategy strategy;
    Plant plant("Test", "014", nullptr, nullptr);
    plant.setNutrientLevel(50);
    strategy.fertilize(&plant);
    EXPECT_EQ(plant.getNutrientLevel(), 60);
}

TEST(OtherPlantCareStrategyTest, AdjustSunlightSetsTo60) {
    OtherPlantCareStrategy strategy;
    Plant plant("Test", "015", nullptr, nullptr);
    strategy.adjustSunlight(&plant);
    EXPECT_EQ(plant.getSunlightExposure(), 60);
}

TEST(OtherPlantCareStrategyTest, PerformCareCallsAllMethods) {
    OtherPlantCareStrategy strategy;
    Plant plant("Test", "016", nullptr, nullptr);
    plant.setWaterLevel(40);
    plant.setNutrientLevel(40);
    strategy.performCare(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 60);
    EXPECT_EQ(plant.getNutrientLevel(), 50);
    EXPECT_EQ(plant.getSunlightExposure(), 60);
}

// ==================== Strategy Comparison Tests ====================

TEST(StrategyComparisonTest, WaterAmountsOrdered) {
    Plant p1("Test", "017", nullptr, nullptr);
    Plant p2("Test", "018", nullptr, nullptr);
    Plant p3("Test", "019", nullptr, nullptr);
    Plant p4("Test", "020", nullptr, nullptr);
    p1.setWaterLevel(50);
    p2.setWaterLevel(50);
    p3.setWaterLevel(50);
    p4.setWaterLevel(50);

    VegetableCareStrategy veg;
    FlowerCareStrategy flower;
    OtherPlantCareStrategy other;
    SucculentCareStrategy succulent;

    veg.water(&p1);
    flower.water(&p2);
    other.water(&p3);
    succulent.water(&p4);

    EXPECT_GE(p1.getWaterLevel(), p2.getWaterLevel());
    EXPECT_GE(p2.getWaterLevel(), p3.getWaterLevel());
    EXPECT_GE(p3.getWaterLevel(), p4.getWaterLevel());
}

TEST(StrategyComparisonTest, FertilizeAmountsOrdered) {
    Plant p1("Test", "021", nullptr, nullptr);
    Plant p2("Test", "022", nullptr, nullptr);
    Plant p3("Test", "023", nullptr, nullptr);
    Plant p4("Test", "024", nullptr, nullptr);
    p1.setNutrientLevel(50);
    p2.setNutrientLevel(50);
    p3.setNutrientLevel(50);
    p4.setNutrientLevel(50);

    VegetableCareStrategy veg;
    FlowerCareStrategy flower;
    OtherPlantCareStrategy other;
    SucculentCareStrategy succulent;

    veg.fertilize(&p1);
    flower.fertilize(&p2);
    other.fertilize(&p3);
    succulent.fertilize(&p4);

    EXPECT_GE(p1.getNutrientLevel(), p2.getNutrientLevel());
    EXPECT_GE(p2.getNutrientLevel(), p3.getNutrientLevel());
    EXPECT_GE(p3.getNutrientLevel(), p4.getNutrientLevel());
}

TEST(StrategyComparisonTest, SunlightSettingsOrdered) {
    Plant p1("Test", "025", nullptr, nullptr);
    Plant p2("Test", "026", nullptr, nullptr);
    Plant p3("Test", "027", nullptr, nullptr);
    Plant p4("Test", "028", nullptr, nullptr);

    SucculentCareStrategy succulent;
    VegetableCareStrategy veg;
    FlowerCareStrategy flower;
    OtherPlantCareStrategy other;

    succulent.adjustSunlight(&p1);
    veg.adjustSunlight(&p2);
    flower.adjustSunlight(&p3);
    other.adjustSunlight(&p4);

    EXPECT_GT(p1.getSunlightExposure(), p2.getSunlightExposure());
    EXPECT_GT(p2.getSunlightExposure(), p3.getSunlightExposure());
    EXPECT_GT(p3.getSunlightExposure(), p4.getSunlightExposure());
}

// ==================== Polymorphism Tests ====================

TEST(PolymorphismTest, BasePointerCallsDerivedMethods) {
    Plant plant("Test", "029", nullptr, nullptr);
    plant.setWaterLevel(50);
    CareStrategy* strategy = new FlowerCareStrategy();
    strategy->water(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 100);
    delete strategy;
}

TEST(PolymorphismTest, DifferentStrategiesProduceDifferentResults) {
    Plant plant1("Test", "030", nullptr, nullptr);
    Plant plant2("Test", "031", nullptr, nullptr);
    plant1.setWaterLevel(50);
    plant2.setWaterLevel(50);
    CareStrategy* strategy1 = new FlowerCareStrategy();
    CareStrategy* strategy2 = new SucculentCareStrategy();
    strategy1->water(&plant1);
    strategy2->water(&plant2);
    EXPECT_NE(plant1.getWaterLevel(), plant2.getWaterLevel());
    delete strategy1;
    delete strategy2;
}

TEST(PolymorphismTest, PerformCareThroughBasePointer) {
    Plant plant("Test", "032", nullptr, nullptr);
    plant.setWaterLevel(40);
    plant.setNutrientLevel(40);
    CareStrategy* strategy = new VegetableCareStrategy();
    strategy->performCare(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 100);
    EXPECT_EQ(plant.getNutrientLevel(), 65);
    delete strategy;
}

// ==================== Edge Case Tests ====================

TEST(EdgeCaseTest, MultipleWaterCallsAccumulate) {
    FlowerCareStrategy strategy;
    Plant plant("Test", "033", nullptr, nullptr);
    plant.setWaterLevel(30);
    strategy.water(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 80);
    strategy.water(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 100);
}

TEST(EdgeCaseTest, SunlightIsAbsoluteNotAdditive) {
    Plant plant("Test", "034", nullptr, nullptr);
    FlowerCareStrategy flower;
    SucculentCareStrategy succulent;
    flower.adjustSunlight(&plant);
    EXPECT_EQ(plant.getSunlightExposure(), 70);
    succulent.adjustSunlight(&plant);
    EXPECT_EQ(plant.getSunlightExposure(), 85);
}

TEST(EdgeCaseTest, MixedStrategiesOnSamePlant) {
    Plant plant("Test", "035", nullptr, nullptr);
    plant.setWaterLevel(30);
    plant.setNutrientLevel(30);
    FlowerCareStrategy flower;
    VegetableCareStrategy veg;
    flower.water(&plant);
    EXPECT_EQ(plant.getWaterLevel(), 80);
    veg.fertilize(&plant);
    EXPECT_EQ(plant.getNutrientLevel(), 55);
    flower.adjustSunlight(&plant);
    EXPECT_EQ(plant.getSunlightExposure(), 70);
}

TEST(EdgeCaseTest, PruneExecutesWithoutError) {
    Plant plant("Test", "036", nullptr, nullptr);
    FlowerCareStrategy strategy;
    EXPECT_NO_THROW(strategy.prune(&plant));
}
