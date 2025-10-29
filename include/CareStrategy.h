#ifndef CARESTRATEGY_H
#define CARESTRATEGY_H

class Plant;

/**
 * @class CareStrategy
 * @brief Abstract base class for plant care strategies
 */
class CareStrategy {
    public:

        /** @brief Virtual destructor */
        virtual ~CareStrategy(){}

        /** @brief Virtual destructor */
        virtual void water(Plant* plant) = 0;

        /** @brief Fertilizes the plant */
        virtual void fertilize(Plant* plant) =0;

        /** @brief Adjusts sunlight exposure */
        virtual void adjustSunlight(Plant* plant) =0;

        /** @brief Prunes the plant */
        virtual void prune(Plant* plant) =0;

        /** @brief Performs complete care routine */
        virtual void performCare(Plant* plant) {
            water(plant);
            fertilize(plant);
            adjustSunlight(plant);
            prune(plant);
        }

};

#endif