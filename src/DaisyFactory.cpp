#include "DaisyFactory.h"
#include "Daisy.h"
#include "FlowerCareStrategy.h"
#include "SeedlingState.h"
#include "WaterObserver.h"
#include "FertilizeObserver.h"
#include "SunlightObserver.h"
#include "CareScheduler.h"

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