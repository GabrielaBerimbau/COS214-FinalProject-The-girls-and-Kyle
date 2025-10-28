#include "include/FlowerCareStrategy.h"
#include "include/Plant.h"
#include <iostream>

void FlowerCareStrategy::water(Plant* plant) {
    // Flowers need specific moisture levels
    plant->setWaterLevel(plant->getWaterLevel() + 25);
    std::cout << "Watering flower - maintaining optimal moisture" << std::endl;
    (void)plant;
}

void FlowerCareStrategy::fertilize(Plant* plant) {
    // Flowers need balanced fertilizer for blooms
    plant->setNutrientLevel(plant->getNutrientLevel() + 20);
    std::cout << "Fertilizing flower - bloom-boosting nutrients" << std::endl;
}

void FlowerCareStrategy::adjustSunlight(Plant* plant) {
    // Flowers need moderate to high sunlight
    plant->setSunlightExposure(70);
    std::cout << "Adjusting flower sunlight - optimal light for blooming" << std::endl;
}

void FlowerCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning flower - deadheading spent blooms" << std::endl;
    (void)plant;
}