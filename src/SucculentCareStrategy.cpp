#include "include/SucculentCareStrategy.h"
#include "include/Plant.h"
#include <iostream>



void SucculentCareStrategy::water(Plant* plant) {
    // Succulents need minimal watering
    plant->setWaterLevel(plant->getWaterLevel() + 15.0);
    std::cout << "Watering succulent - requires minimal water" << std::endl;
    (void)plant;
}


void SucculentCareStrategy::fertilize(Plant* plant) {
    // Succulents need minimal fertilizer
   plant->setNutrientLevel(plant->getNutrientLevel() + 10);
    std::cout << "Fertilizing succulent - light feeding" << std::endl;
}


void SucculentCareStrategy::adjustSunlight(Plant* plant) {
    // Succulents love lots of sunlight
    plant->setSunlightExposure(85.0);
    std::cout << "Adjusting succulent sunlight - high exposure preferred" << std::endl;
}

void SucculentCareStrategy::prune(Plant* plant) {
    std::cout << "Pruning succulent - removing dead leaves" << std::endl;
    (void)plant;
}