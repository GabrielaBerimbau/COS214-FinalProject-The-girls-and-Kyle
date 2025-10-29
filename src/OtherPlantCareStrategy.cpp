#include "include/OtherPlantCareStrategy.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @brief Waters general plants with standard care
 * 
 * Applies a moderate amount of water (20 units) suitable for plants
 * that don't have specialized watering requirements.
 * 
 * @param plant Pointer to the plant to be watered
 */
void OtherPlantCareStrategy::water(Plant* plant) {
    // Standard watering for other plant types
    plant->setWaterLevel(plant->getWaterLevel() + 20);
    std::cout << "Watering plant - standard care" << std::endl;
    (void)plant;
}

/**
 * @brief Fertilizes general plants with standard feeding
 * 
 * Applies a balanced, all-purpose fertilizer increasing nutrient levels
 * by 15 units.
 * 
 * @param plant Pointer to the plant to be fertilized
 */
void OtherPlantCareStrategy::fertilize(Plant* plant) {
    // Standard fertilization
    plant->setNutrientLevel(plant->getNutrientLevel() + 15);
    std::cout << "Fertilizing plant - standard feeding" << std::endl;
}

/**
 * @brief Adjusts sunlight to moderate levels
 * 
 * Sets sunlight exposure to 60% (moderate), which is appropriate for
 * plants that tolerate a range of light conditions.
 * 
 * @param plant Pointer to the plant requiring sunlight adjustment
 */
void OtherPlantCareStrategy::adjustSunlight(Plant* plant) {
    // Moderate sunlight for other plants
    plant->setSunlightExposure(60);
    std::cout << "Adjusting plant sunlight - moderate exposure" << std::endl;
}

/**
 * @brief Prunes plants for general health maintenance
 * 
 * Performs basic pruning to remove dead, damaged, or overgrown foliage.
 * 
 * @param plant Pointer to the plant to be pruned
 */
void OtherPlantCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning plant - general maintenance" << std::endl;
(void)plant;
}
