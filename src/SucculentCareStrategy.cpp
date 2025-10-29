#include "include/SucculentCareStrategy.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @brief Waters succulents with minimal moisture
 * 
 * Succulents store water in their leaves and stems, requiring infrequent
 * watering. This method adds only 15 units of water, preventing root rot
 * which is the most common cause of succulent death.
 * 
 * @param plant Pointer to the succulent plant to be watered
 */
void SucculentCareStrategy::water(Plant* plant) {
    // Succulents need minimal watering
    plant->setWaterLevel(plant->getWaterLevel() + 15.0);
    std::cout << "Watering succulent - requires minimal water" << std::endl;
    (void)plant;
}

/**
 * @brief Fertilizes succulents with light feeding
 * 
 * Succulents are adapted to nutrient-poor environments and require
 * minimal fertilization. Adds only 10 units of nutrients.
 * 
 * @param plant Pointer to the succulent plant to be fertilized
 */
void SucculentCareStrategy::fertilize(Plant* plant) {
    // Succulents need minimal fertilizer
   plant->setNutrientLevel(plant->getNutrientLevel() + 10);
    std::cout << "Fertilizing succulent - light feeding" << std::endl;
}

/**
 * @brief Adjusts sunlight to high exposure levels
 * 
 * Sets sunlight exposure to 85% (high), reflecting succulents' natural
 * habitat in arid, sunny environments.
 * 
 * @param plant Pointer to the succulent plant requiring sunlight adjustment
 */
void SucculentCareStrategy::adjustSunlight(Plant* plant) {
    // Succulents love lots of sunlight
    plant->setSunlightExposure(85.0);
    std::cout << "Adjusting succulent sunlight - high exposure preferred" << std::endl;
}

/**
 * @brief Prunes succulents by removing dead leaves
 * 
 * Removes dried, shriveled, or damaged leaves from the base of the
 * succulent.
 * 
 * @param plant Pointer to the succulent plant to be pruned
 */
void SucculentCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning succulent - removing dead leaves" << std::endl;
    (void)plant;
}