#include "include/VenusFlyTrapFactory.h"
#include "include/VenusFlyTrap.h"
#include "include/OtherPlantCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* VenusFlyTrapFactory::buildPlant(CareScheduler* scheduler) const {
    static int vftCounter = 1;
    std::string plantId = "VFT_" + std::to_string(vftCounter++);
    CareStrategy* careStrategy = new OtherPlantCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    VenusFlyTrap* plant = new VenusFlyTrap(plantId, careStrategy, initialState, 5);
    
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