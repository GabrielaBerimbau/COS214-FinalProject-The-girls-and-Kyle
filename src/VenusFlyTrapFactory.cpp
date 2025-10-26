#include "include/VenusFlyTrapFactory.h"
#include "include/VenusFlyTrap.h"
#include "include/OtherPlantCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* VenusFlyTrapFactory::buildPlant() const {
    static int vftCounter = 1;
    std::string plantId = "VFT_" + std::to_string(vftCounter++);
    CareStrategy* careStrategy = new OtherPlantCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    VenusFlyTrap* plant = new VenusFlyTrap(plantId, careStrategy, initialState, 5);
    
    CareScheduler* scheduler = getScheduler();
    
    // Create observers, attach() is in constructors for observers
    new WaterObserver(scheduler, plant);
    new FertilizeObserver(scheduler, plant);
    new SunlightObserver(scheduler, plant);
    
    return plant;
}