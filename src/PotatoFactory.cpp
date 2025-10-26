#include "PotatoFactory.h"
#include "Potato.h"
#include "VegetableCareStrategy.h"
#include "SeedlingState.h"
#include "WaterObserver.h"
#include "FertilizeObserver.h"
#include "SunlightObserver.h"
#include "CareScheduler.h"

Plant* PotatoFactory::buildPlant() const {
    static int potatoCounter = 1;
    std::string plantId = "POTATO_" + std::to_string(potatoCounter++);
    CareStrategy* careStrategy = new VegetableCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Potato* plant = new Potato(plantId, careStrategy, initialState, "Russet", "Brown");
    
    CareScheduler* scheduler = getScheduler();
    
    // Create observers, attach() is in constructors for observers
    new WaterObserver(scheduler, plant);
    new FertilizeObserver(scheduler, plant);
    new SunlightObserver(scheduler, plant);
    
    return plant;
}