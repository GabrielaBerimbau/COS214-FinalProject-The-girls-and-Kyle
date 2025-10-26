#ifndef SUCCULENTCARESTRATEGY_H
#define SUCCULENTCARESTRATEGY_H

#include "CareStrategy.h"
#include <iostream>


class SucculentCareStrategy : public CareStrategy {
    public:
        void water(Plant* plant) override;
        void fertilize(Plant* plant) override;
        void adjustSunlight(Plant* plant) override;
        void prune(Plant* plant) override;
};

#endif