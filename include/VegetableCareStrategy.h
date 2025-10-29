#ifndef VEGETABLECARESTRATEGY_H
#define VEGETABLECARESTRATEGY_H

#include "CareStrategy.h"

/**
 * @class VegetableCareStrategy
 * @brief Care strategy for vegetable plants
 */
class VegetableCareStrategy : public CareStrategy {
public:
    void water(Plant* plant) override;
    void fertilize(Plant* plant) override;
    void adjustSunlight(Plant* plant) override;
    void prune(Plant* plant) override;
};

#endif
