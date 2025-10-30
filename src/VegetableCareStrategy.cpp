#include "include/VegetableCareStrategy.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @author Rachel Clifford
 * @date October 26, 2025
 * @class VegetableCareStrategy
 * @brief Care strategy for vegetable plants
 */

/**
 * @brief Waters vegetable plants on regular schedule 
 */
void VegetableCareStrategy::water(Plant* plant) {
    // Vegetables need regular, moderate watering
    plant->setWaterLevel(plant->getWaterLevel() + 30);
    std::cout << "Watering vegetable - regular watering schedule" << std::endl;
    (void)plant;
}

/**
 * @brief Fertilizes vegetables with nutrient-rich feeding 
 */
void VegetableCareStrategy::fertilize(Plant* plant) {
    // Vegetables are heavy feeders
    plant->setNutrientLevel(plant->getNutrientLevel() + 25);
    std::cout << "Fertilizing vegetable - nutrient-rich feeding" << std::endl;
}

/**
 * @brief Adjusts sunlight for full sun exposure 
 */
void VegetableCareStrategy::adjustSunlight(Plant* plant) {
    // Vegetables need good sunlight
    plant->setSunlightExposure(75);
    std::cout << "Adjusting vegetable sunlight - full sun exposure" << std::endl;
}

/**
 * @brief Prunes vegetables to improve productivity 
 */
void VegetableCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning vegetable - removing old growth and suckers" << std::endl;
(void)plant;
}