#ifndef FLOWERCARESTRATEGY_H
#define FLOWERCARESTRATEGY_H

#include "CareStrategy.h"

/**
 * @class FlowerCareStrategy
 * @brief Care strategy for flowering plants
 */
class FlowerCareStrategy : public CareStrategy {
public:
    void water(Plant* plant) override;
    void fertilize(Plant* plant) override;
    void adjustSunlight(Plant* plant) override;
    void prune(Plant* plant) override;
};

#endif