#include "include/VegetableCareStrategy.h"
#include "include/Plant.h"
#include <iostream>


void VegetableCareStrategy::water(Plant* plant) {
    // Vegetables need regular, moderate watering
    plant->setWaterLevel(plant->getWaterLevel() + 100);
    std::cout << "Watering vegetable - regular watering schedule" << std::endl;
    (void)plant;
}


void VegetableCareStrategy::fertilize(Plant* plant) {
    // Vegetables are heavy feeders
    plant->setNutrientLevel(plant->getNutrientLevel() + 25);
    std::cout << "Fertilizing vegetable - nutrient-rich feeding" << std::endl;
}


void VegetableCareStrategy::adjustSunlight(Plant* plant) {
    // Vegetables need good sunlight
    plant->setSunlightExposure(75);
    std::cout << "Adjusting vegetable sunlight - full sun exposure" << std::endl;
}


void VegetableCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning vegetable - removing old growth and suckers" << std::endl;
(void)plant;
}