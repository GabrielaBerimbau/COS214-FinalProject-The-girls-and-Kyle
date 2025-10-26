
#include "include/AloeFactory.h"
#include "include/Aloe.h"
#include "include/SucculentCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"
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