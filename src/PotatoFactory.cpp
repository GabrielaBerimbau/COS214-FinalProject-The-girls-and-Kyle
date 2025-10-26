#include "include/PotatoFactory.h"
#include "include/Potato.h"
#include "include/VegetableCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

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