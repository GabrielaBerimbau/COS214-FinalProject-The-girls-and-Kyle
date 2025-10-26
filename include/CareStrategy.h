#ifndef CARESTRATEGY_H
#define CARESTRATEGY_H

class Plant;

class CareStrategy {
    public:
        virtual ~CareStrategy(){}

        virtual void water(Plant* plant) = 0;

        virtual void fertilize(Plant* plant) =0;

        virtual void adjustSunlight(Plant* plant) =0;

        virtual void prune(Plant* plant) =0;

        virtual void performCare(Plant* plant) {
            water(plant);
            fertilize(plant);
            adjustSunlight(plant);
            prune(plant);
        }

};

#endif