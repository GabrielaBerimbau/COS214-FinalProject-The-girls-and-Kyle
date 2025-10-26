
#include "AloeFactory.h"
#include "Aloe.h"
#include "SucculentCareStrategy.h"
#include "SeedlingState.h"
#include "WaterObserver.h"
#include "FertilizeObserver.h"
#include "SunlightObserver.h"
#include "CareScheduler.h"
#include <iostream>

Plant* AloeFactory::buildPlant() const {

    static int aloeCounter = 1;
    std::string plantId = "ALOE_" + std::to_string(aloeCounter++);
    CareStrategy* careStrategy = new SucculentCareStrategy();
    PlantState* initialState = new SeedlingState();
    
  
    Aloe* plant = new Aloe(plantId, careStrategy, initialState, "Vera");
    
    
    
    CareScheduler* scheduler = getScheduler();
    
    // Create observers, attach() is in constructors for observers
    new WaterObserver(scheduler, plant);
    new FertilizeObserver(scheduler, plant);
    new SunlightObserver(scheduler, plant);


    
    return plant;
}