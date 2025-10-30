#ifndef VEGETABLECARESTRATEGY_H
#define VEGETABLECARESTRATEGY_H

#include "CareStrategy.h"

/**
 * @author Rachel Clifford
 * @date October 26, 2025
 * @class VegetableCareStrategy
 * @brief Care strategy for vegetable plants
 */
class VegetableCareStrategy : public CareStrategy {
public:

    /**
     * @brief Waters vegetable plants on regular schedule
     * 
     * Vegetables require consistent, moderate watering to produce healthy
     * crops. Adds 30 units of water to maintain steady soil moisture without
     * waterlogging.
     * 
     * @param plant Pointer to the vegetable plant to be watered
     */
    void water(Plant* plant) override;

    /**
     * @brief Fertilizes vegetables with nutrient-rich feeding
     * 
     * Vegetables are heavy feeders that require significant nutrients to
     * produce fruits and vegetables. Increases nutrient level by 25 units.
     * 
     * @param plant Pointer to the vegetable plant to be fertilized
     */
    void fertilize(Plant* plant) override;

    /**
     * @brief Adjusts sunlight for full sun exposure
     * 
     * Sets sunlight exposure to 75% (full sun), which is essential for most
     * vegetable crops.
     * 
     * @param plant Pointer to the vegetable plant requiring sunlight adjustment
     */
    void adjustSunlight(Plant* plant) override;

    /**
     * @brief Prunes vegetables to improve productivity
     * 
     * Removes old growth, dead leaves, and suckers (non-fruiting shoots)
     * that divert energy from fruit production.
     * 
     * @param plant Pointer to the vegetable plant to be pruned
     */
    void prune(Plant* plant) override;
};

#endif
