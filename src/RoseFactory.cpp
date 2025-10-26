#include "include/RoseFactory.h"
#include "include/Rose.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/FlowerCareStrategy.h"
#include "include/CareScheduler.h"

Plant* RoseFactory::buildPlant() const {
    static int roseCounter = 1;
    std::string plantId = "ROSE_" + std::to_string(roseCounter++);
    CareStrategy* careStrategy = new FlowerCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Rose* plant = new Rose(plantId, careStrategy, initialState, "Red", "Hybrid Tea");
    
    CareScheduler* scheduler = getScheduler();
    
    // Create observers, attach() is in constructors for observers
    new WaterObserver(scheduler, plant);
    new FertilizeObserver(scheduler, plant);
    new SunlightObserver(scheduler, plant);
    
    return plant;
}