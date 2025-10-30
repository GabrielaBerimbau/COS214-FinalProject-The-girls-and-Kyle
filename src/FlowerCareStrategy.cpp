#include "include/FlowerCareStrategy.h"
#include "include/Plant.h"
#include <iostream>


/**
 * @author Rachel Clifford
 * @date October 26, 2025
 * @class FlowerCareStrategy
 * @brief Care strategy for flowering plants
 */


 /**
 * @brief Waters flowering plants with optimal moisture levels
 */ 
void FlowerCareStrategy::water(Plant* plant) {
    // Flowers need specific moisture levels
    plant->setWaterLevel(plant->getWaterLevel() + 25);
    std::cout << "Watering flower - maintaining optimal moisture" << std::endl;
    (void)plant;
}

/**
 *  @brief Fertilizes flowering plants with bloom-boosting nutrients
 */ 
void FlowerCareStrategy::fertilize(Plant* plant) {
    // Flowers need balanced fertilizer for blooms
    plant->setNutrientLevel(plant->getNutrientLevel() + 20);
    std::cout << "Fertilizing flower - bloom-boosting nutrients" << std::endl;
}

/**
 * @brief Adjusts sunlight exposure for optimal flowering
 */ 
void FlowerCareStrategy::adjustSunlight(Plant* plant) {
    // Flowers need moderate to high sunlight
    plant->setSunlightExposure(70);
    std::cout << "Adjusting flower sunlight - optimal light for blooming" << std::endl;
}

/**
 * @brief Prunes flowering plants by removing spent blooms
 */ 
void FlowerCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning flower - deadheading spent blooms" << std::endl;
    (void)plant;
}