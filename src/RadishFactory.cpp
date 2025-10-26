#include "RadishFactory.h"
#include "Radish.h"
#include "VegetableCareStrategy.h"
#include "SeedlingState.h"
#include "WaterObserver.h"
#include "FertilizeObserver.h"
#include "SunlightObserver.h"
#include "CareScheduler.h"

Plant* RadishFactory::buildPlant() const {
    static int radishCounter = 1;
    std::string plantId = "RADISH_" + std::to_string(radishCounter++);
    CareStrategy* careStrategy = new VegetableCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Radish* plant = new Radish(plantId, careStrategy, initialState, "Cherry Belle", "Red");
    
    CareScheduler* scheduler = getScheduler();
    
    // Create observers, attach() is in constructors for observers
    new WaterObserver(scheduler, plant);
    new FertilizeObserver(scheduler, plant);
    new SunlightObserver(scheduler, plant);
    
    return plant;
}