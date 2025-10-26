#include "include/RadishFactory.h"
#include "include/Radish.h"
#include "include/VegetableCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

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