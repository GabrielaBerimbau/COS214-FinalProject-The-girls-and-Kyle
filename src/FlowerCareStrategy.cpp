#include "include/FlowerCareStrategy.h"
#include "include/Plant.h"
#include <iostream>


/**
 * @brief Waters flowering plants with optimal moisture levels
 * 
 * Flowers require specific moisture levels to maintain healthy blooms.
 * This method increases the plant's water level by 25 units.
 * 
 * @param plant Pointer to the flower plant to be watered
 */
void FlowerCareStrategy::water(Plant* plant) {
    // Flowers need specific moisture levels
    plant->setWaterLevel(plant->getWaterLevel() + 25);
    std::cout << "Watering flower - maintaining optimal moisture" << std::endl;
    (void)plant;
}

/**
 * @brief Fertilizes flowering plants with bloom-boosting nutrients
 * 
 * Applies balanced fertilizer specifically formulated to promote flower
 * production. Increases nutrient level by 20 units
 * 
 * @param plant Pointer to the flower plant to be fertilized
 */
void FlowerCareStrategy::fertilize(Plant* plant) {
    // Flowers need balanced fertilizer for blooms
    plant->setNutrientLevel(plant->getNutrientLevel() + 20);
    std::cout << "Fertilizing flower - bloom-boosting nutrients" << std::endl;
}

/**
 * @brief Adjusts sunlight exposure for optimal flowering
 * 
 * Sets sunlight exposure to 70% (moderate to high), which is ideal for
 * most flowering plants.
 * 
 * @param plant Pointer to the flower plant requiring sunlight adjustment
 */
void FlowerCareStrategy::adjustSunlight(Plant* plant) {
    // Flowers need moderate to high sunlight
    plant->setSunlightExposure(70);
    std::cout << "Adjusting flower sunlight - optimal light for blooming" << std::endl;
}

/**
 * @brief Prunes flowering plants by removing spent blooms
 * 
 * Performs deadheading - the removal of faded or dead flowers. This
 * practice encourages the plant to produce new blooms.
 * 
 * @param plant Pointer to the flower plant to be pruned
 */
void FlowerCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning flower - deadheading spent blooms" << std::endl;
    (void)plant;
}