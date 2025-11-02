#ifndef OTHERPLANTCARESTRATEGY_H
#define OTHERPLANTCARESTRATEGY_H

#include "CareStrategy.h"

/**
 * @author Rachel Clifford
 * @date October 26, 2025
 * @class OtherPlantCareStrategy
 * @brief General care strategy for miscellaneous plants
 */
class OtherPlantCareStrategy : public CareStrategy {
public:
    /**
     * @brief Waters general plants with standard care
     * 
     * Applies a moderate amount of water (20 units) suitable for plants
     * that don't have specialized watering requirements.
     * 
     * @param plant Pointer to the plant to be watered
     */
    void water(Plant* plant) override;

    /**
     * @brief Fertilizes general plants with standard feeding
     * 
     * Applies a balanced, all-purpose fertilizer increasing nutrient levels
     * by 15 units.
     * 
     * @param plant Pointer to the plant to be fertilized
     */
    void fertilize(Plant* plant) override;

    /**
     * @brief Adjusts sunlight to moderate levels
     * 
     * Sets sunlight exposure to 60% (moderate), which is appropriate for
     * plants that tolerate a range of light conditions.
     * 
     * @param plant Pointer to the plant requiring sunlight adjustment
     */
    void adjustSunlight(Plant* plant) override;

    
    /**
     * @brief Prunes plants for general health maintenance
     * 
     * Performs basic pruning to remove dead, damaged, or overgrown foliage.
     * 
     * @param plant Pointer to the plant to be pruned
     */
    void prune(Plant* plant) override;
};

#endif