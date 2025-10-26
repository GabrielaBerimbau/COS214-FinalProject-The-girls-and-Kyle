#include "include/DaisyFactory.h"
#include "include/Daisy.h"
#include "include/FlowerCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* DaisyFactory::buildPlant() const {
    static int daisyCounter = 1;
    std::string plantId = "DAISY_" + std::to_string(daisyCounter++);
    CareStrategy* careStrategy = new FlowerCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Daisy* plant = new Daisy(plantId, careStrategy, initialState, "White", "Common");
    
    CareScheduler* scheduler = getScheduler();
    
    // Create observers, attach() is in constructors for observers
    new WaterObserver(scheduler, plant);
    new FertilizeObserver(scheduler, plant);
    new SunlightObserver(scheduler, plant);
    
    return plant;
}