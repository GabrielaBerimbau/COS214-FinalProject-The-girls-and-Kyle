#ifndef SUCCULENTCARESTRATEGY_H
#define SUCCULENTCARESTRATEGY_H

#include "CareStrategy.h"
#include <iostream>

/**
 * @author Rachel Clifford
 * @date October 26, 2025
 * @class SucculentCareStrategy
 * @brief Care strategy for succulent plants
 */
class SucculentCareStrategy : public CareStrategy {
    public:

        /**
         * @brief Waters succulents with minimal moisture
         * 
         * Succulents store water in their leaves and stems, requiring infrequent
         * watering. This method adds only 15 units of water, preventing root rot
         * which is the most common cause of succulent death.
         * 
         * @param plant Pointer to the succulent plant to be watered
         */
        void water(Plant* plant) override;

        /**
         * @brief Fertilizes succulents with light feeding
         * 
         * Succulents are adapted to nutrient-poor environments and require
         * minimal fertilization. Adds only 10 units of nutrients.
         * 
         * @param plant Pointer to the succulent plant to be fertilized
         */
        void fertilize(Plant* plant) override;

        /**
         * @brief Adjusts sunlight to high exposure levels
         * 
         * Sets sunlight exposure to 85% (high), reflecting succulents' natural
         * habitat in arid, sunny environments.
         * 
         * @param plant Pointer to the succulent plant requiring sunlight adjustment
         */
        void adjustSunlight(Plant* plant) override;

        /**
         * @brief Prunes succulents by removing dead leaves
         * 
         * Removes dried, shriveled, or damaged leaves from the base of the
         * succulent.
         * 
         * @param plant Pointer to the succulent plant to be pruned
         */
        void prune(Plant* plant) override;
};

#endif