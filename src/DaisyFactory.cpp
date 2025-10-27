#include "include/DaisyFactory.h"
#include "include/Daisy.h"
#include "include/FlowerCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* DaisyFactory::buildPlant(CareScheduler* scheduler) const {
    static int daisyCounter = 1;
    std::string plantId = "DAISY_" + std::to_string(daisyCounter++);
    CareStrategy* careStrategy = new FlowerCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Daisy* plant = new Daisy(plantId, careStrategy, initialState, "White", "Common");
    
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