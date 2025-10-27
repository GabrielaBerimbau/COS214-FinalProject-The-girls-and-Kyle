#include "include/RoseFactory.h"
#include "include/Rose.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/FlowerCareStrategy.h"
#include "include/CareScheduler.h"

Plant* RoseFactory::buildPlant(CareScheduler* scheduler) const {
    static int roseCounter = 1;
    std::string plantId = "ROSE_" + std::to_string(roseCounter++);
    CareStrategy* careStrategy = new FlowerCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Rose* plant = new Rose(plantId, careStrategy, initialState, "Red", "Hybrid Tea");
    
    // Only create observers if scheduler is provided
    if (scheduler != nullptr) {
        WaterObserver* waterObs = new WaterObserver(scheduler, plant);
        FertilizeObserver* fertObs = new FertilizeObserver(scheduler, plant);
        SunlightObserver* sunObs = new SunlightObserver(scheduler, plant);
        
        // Plant takes ownership and will delete these
        plant->addOwnedObserver(waterObs);
        plant->addOwnedObserver(fertObs);
        plant->addOwnedObserver(sunObs);
    }
    
    return plant;
}