#ifndef FLOWERCARESTRATEGY_H
#define FLOWERCARESTRATEGY_H

#include "CareStrategy.h"

/**
 * @author Rachel Clifford
 * @date October 26, 2025
 * @class FlowerCareStrategy
 * @brief Care strategy for flowering plants
 */
class FlowerCareStrategy : public CareStrategy {
public:

    /**
     * @brief Waters flowering plants with optimal moisture levels
     * 
     * Flowers require specific moisture levels to maintain healthy blooms.
     * This method increases the plant's water level by 25 units.
     * 
     * @param plant Pointer to the flower plant to be watered
     */
    void water(Plant* plant) override;

    /**
     * @brief Fertilizes flowering plants with bloom-boosting nutrients
     * 
     * Applies balanced fertilizer specifically formulated to promote flower
     * production. Increases nutrient level by 20 units
     * 
     * @param plant Pointer to the flower plant to be fertilized
     */
    void fertilize(Plant* plant) override;

    /**
     * @brief Adjusts sunlight exposure for optimal flowering
     * 
     * Sets sunlight exposure to 70% (moderate to high), which is ideal for
     * most flowering plants.
     * 
     * @param plant Pointer to the flower plant requiring sunlight adjustment
     */
    void adjustSunlight(Plant* plant) override;

    /**
     * @brief Prunes flowering plants by removing spent blooms
     * 
     * Performs deadheading - the removal of faded or dead flowers. This
     * practice encourages the plant to produce new blooms.
     * 
     * @param plant Pointer to the flower plant to be pruned
     */
    void prune(Plant* plant) override;
};

#endif