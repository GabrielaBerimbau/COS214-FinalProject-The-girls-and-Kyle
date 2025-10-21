#ifndef CARESTRATEGY_H
#define CARESTRATEGY_H

class CareStrategy {
    public:
        virtual ~CareStrategy(){}

        virtual void water() = 0;

        virtual void fertilize() =0;

        virtual void adjustSunlight() =0;

        virtual void prune() =0;

};

#endif