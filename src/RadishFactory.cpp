#include "include/RadishFactory.h"
#include "include/Radish.h"
#include "include/VegetableCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* RadishFactory::buildPlant(CareScheduler* scheduler) const {
    static int radishCounter = 1;
    std::string plantId = "RADISH_" + std::to_string(radishCounter++);
    CareStrategy* careStrategy = new VegetableCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Radish* plant = new Radish(plantId, careStrategy, initialState, "Cherry Belle", "Red");
    
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