#ifndef SUCCULENTCARESTRATEGY_H
#define SUCCULENTCARESTRATEGY_H

#include "CareStrategy.h"
#include <iostream>


class SucculentCareStrategy : public CareStrategy {
    public:
        void water() override;
        void fertilize() override;
        void adjustSunlight() override;
        void prune() override;
};

#endif