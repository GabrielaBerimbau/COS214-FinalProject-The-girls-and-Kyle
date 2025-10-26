#ifndef OTHERPLANTCARESTRATEGY_H
#define OTHERPLANTCARESTRATEGY_H

#include "CareStrategy.h"

class OtherPlantCareStrategy : public CareStrategy {
public:
    void water(Plant* plant) override;
    void fertilize(Plant* plant) override;
    void adjustSunlight(Plant* plant) override;
    void prune(Plant* plant) override;
};

#endif