//do we need this??

#ifndef ROSECARESTRATEGY_H
#define ROSECARESTRATEGY_H

#include "CareStrategy.h"
#include <iostream>

class RoseCareStrategy : public CareStrategy {
    public:
        void water(Plant* plant) override;
        void fertilize(Plant* plant) override;
        void adjustSunlight(Plant* plant) override;
        void prune(Plant* plant) override;

};

#endif