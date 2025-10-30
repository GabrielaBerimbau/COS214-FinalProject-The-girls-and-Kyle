#include "include/SucculentCareStrategy.h"
#include "include/Plant.h"
#include <iostream>


/**
 * @author Rachel Clifford
 * @date October 26, 2025
 * @class SucculentCareStrategy
 * @brief Care strategy for succulent plants
 */

/**
 * @brief Waters succulents with minimal moisture 
 */
void SucculentCareStrategy::water(Plant* plant) {
    // Succulents need minimal watering
    plant->setWaterLevel(plant->getWaterLevel() + 15.0);
    std::cout << "Watering succulent - requires minimal water" << std::endl;
    (void)plant;
}

 /**
 * @brief Fertilizes succulents with light feeding 
 */
void SucculentCareStrategy::fertilize(Plant* plant) {
    // Succulents need minimal fertilizer
   plant->setNutrientLevel(plant->getNutrientLevel() + 10);
    std::cout << "Fertilizing succulent - light feeding" << std::endl;
}

/**
 * @brief Adjusts sunlight to high exposure levels 
 */
void SucculentCareStrategy::adjustSunlight(Plant* plant) {
    // Succulents love lots of sunlight
    plant->setSunlightExposure(85.0);
    std::cout << "Adjusting succulent sunlight - high exposure preferred" << std::endl;
}

/**
 *  @brief Prunes succulents by removing dead leaves 
 */
void SucculentCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning succulent - removing dead leaves" << std::endl;
    (void)plant;
}