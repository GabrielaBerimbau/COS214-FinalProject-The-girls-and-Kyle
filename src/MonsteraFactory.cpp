#include "include/MonsteraFactory.h"
#include "include/Monstera.h"
#include "include/OtherPlantCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* MonsteraFactory::buildPlant(CareScheduler* scheduler) const {
    static int monsteraCounter = 1;
    std::string plantId = "MONSTERA_" + std::to_string(monsteraCounter++);
    CareStrategy* careStrategy = new OtherPlantCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Monstera* plant = new Monstera(plantId, careStrategy, initialState, 3);
    
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
