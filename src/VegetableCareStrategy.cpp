#include "include/VegetableCareStrategy.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @brief Waters vegetable plants on regular schedule
 * 
 * Vegetables require consistent, moderate watering to produce healthy
 * crops. Adds 30 units of water to maintain steady soil moisture without
 * waterlogging.
 * 
 * @param plant Pointer to the vegetable plant to be watered
 */
void VegetableCareStrategy::water(Plant* plant) {
    // Vegetables need regular, moderate watering
    plant->setWaterLevel(plant->getWaterLevel() + 100);
    std::cout << "Watering vegetable - regular watering schedule" << std::endl;
    (void)plant;
}

/**
 * @brief Fertilizes vegetables with nutrient-rich feeding
 * 
 * Vegetables are heavy feeders that require significant nutrients to
 * produce fruits and vegetables. Increases nutrient level by 25 units.
 * 
 * @param plant Pointer to the vegetable plant to be fertilized
 */
void VegetableCareStrategy::fertilize(Plant* plant) {
    // Vegetables are heavy feeders
    plant->setNutrientLevel(plant->getNutrientLevel() + 25);
    std::cout << "Fertilizing vegetable - nutrient-rich feeding" << std::endl;
}

/**
 * @brief Adjusts sunlight for full sun exposure
 * 
 * Sets sunlight exposure to 75% (full sun), which is essential for most
 * vegetable crops.
 * 
 * @param plant Pointer to the vegetable plant requiring sunlight adjustment
 */
void VegetableCareStrategy::adjustSunlight(Plant* plant) {
    // Vegetables need good sunlight
    plant->setSunlightExposure(75);
    std::cout << "Adjusting vegetable sunlight - full sun exposure" << std::endl;
}

/**
 * @brief Prunes vegetables to improve productivity
 * 
 * Removes old growth, dead leaves, and suckers (non-fruiting shoots)
 * that divert energy from fruit production.
 * 
 * @param plant Pointer to the vegetable plant to be pruned
 */
void VegetableCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning vegetable - removing old growth and suckers" << std::endl;
(void)plant;
}