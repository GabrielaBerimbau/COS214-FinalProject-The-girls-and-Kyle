#include "VenusFlyTrapFactory.h"
#include "VenusFlyTrap.h"
#include "OtherPlantCareStrategy.h"
#include "SeedlingState.h"
#include "WaterObserver.h"
#include "FertilizeObserver.h"
#include "SunlightObserver.h"
#include "CareScheduler.h"

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