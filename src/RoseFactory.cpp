#include "RoseFactory.h"
#include "Rose.h"
#include "RoseCareStrategy.h"
#include "SeedlingState.h"
#include "WaterObserver.h"
#include "FertilizeObserver.h"
#include "SunlightObserver.h"
#include "CareScheduler.h"

Plant* RoseFactory::buildPlant() const {
    static int roseCounter = 1;
    std::string plantId = "ROSE_" + std::to_string(roseCounter++);
    CareStrategy* careStrategy = new RoseCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Rose* plant = new Rose(plantId, careStrategy, initialState, "Red", "Hybrid Tea");
    
    CareScheduler* scheduler = getScheduler();
    
    // Create observers, attach() is in constructors for observers
    new WaterObserver(scheduler, plant);
    new FertilizeObserver(scheduler, plant);
    new SunlightObserver(scheduler, plant);
    
    return plant;
}