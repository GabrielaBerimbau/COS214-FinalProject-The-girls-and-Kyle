#include "include/StrelitziaFactory.h"
#include "include/Strelitzia.h"
#include "include/FlowerCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* StrelitziaFactory::buildPlant(CareScheduler* scheduler) const {
    static int strelitziaCounter = 1;
    std::string plantId = "STRELITZIA_" + std::to_string(strelitziaCounter++);
    CareStrategy* careStrategy = new FlowerCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Strelitzia* plant = new Strelitzia(plantId, careStrategy, initialState);
    
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