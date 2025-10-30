#include "include/OtherPlantCareStrategy.h"
#include "include/Plant.h"
#include <iostream>



 /**
 * @author Rachel Clifford
 * @date October 26, 2025
 * @class OtherPlantCareStrategy
 * @brief General care strategy for miscellaneous plants
 */

void OtherPlantCareStrategy::water(Plant* plant) {
    // Standard watering for other plant types
    plant->setWaterLevel(plant->getWaterLevel() + 20);
    std::cout << "Watering plant - standard care" << std::endl;
    (void)plant;
}


void OtherPlantCareStrategy::fertilize(Plant* plant) {
    // Standard fertilization
    plant->setNutrientLevel(plant->getNutrientLevel() + 15);
    std::cout << "Fertilizing plant - standard feeding" << std::endl;
}


void OtherPlantCareStrategy::adjustSunlight(Plant* plant) {
    // Moderate sunlight for other plants
    plant->setSunlightExposure(60);
    std::cout << "Adjusting plant sunlight - moderate exposure" << std::endl;
}


void OtherPlantCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning plant - general maintenance" << std::endl;
(void)plant;
}
