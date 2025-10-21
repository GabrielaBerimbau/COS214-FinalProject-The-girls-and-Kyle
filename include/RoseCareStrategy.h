#ifndef ROSECARESTRATEGY_H
#define ROSECARESTRATEGY_H

#include "CareStrategy.h"
#include <iostream>

class RoseCareStrategy : public CareStrategy {
    public:
        void water() override;
        void fertilize() override;
        void adjustSunlight() override;
        void prune() override;

};

#endif