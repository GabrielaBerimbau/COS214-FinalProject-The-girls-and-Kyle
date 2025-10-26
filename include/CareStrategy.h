#ifndef CARESTRATEGY_H
#define CARESTRATEGY_H

class Plant;

class CareStrategy {
    public:
        virtual ~CareStrategy(){}

        virtual void performCare(Plant* plant) = 0;

        virtual void water(Plant* plant) = 0;

        virtual void fertilize(Plant* plant) =0;

        virtual void adjustSunlight(Plant* plant) =0;

        virtual void prune(Plant* plant) =0;

};

#endif