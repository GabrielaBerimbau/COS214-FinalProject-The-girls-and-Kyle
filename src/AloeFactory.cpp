#include "include/AloeFactory.h"
#include "include/Aloe.h"
#include "include/SucculentCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"
#include <iostream>

Plant* AloeFactory::buildPlant(CareScheduler* scheduler) const {
    static int aloeCounter = 1;
    std::string plantId = "ALOE_" + std::to_string(aloeCounter++);
    CareStrategy* careStrategy = new SucculentCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Aloe* plant = new Aloe(plantId, careStrategy, initialState, "Vera");
    
    if (scheduler != nullptr) {
        WaterObserver* waterObs = new WaterObserver(scheduler, plant);
        FertilizeObserver* fertObs = new FertilizeObserver(scheduler, plant);
        SunlightObserver* sunObs = new SunlightObserver(scheduler, plant);
        
        plant->addOwnedObserver(waterObs);
        plant->addOwnedObserver(fertObs);
        plant->addOwnedObserver(sunObs);
    }
    
    return plant;
}