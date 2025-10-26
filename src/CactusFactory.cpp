#include "include/CactusFactory.h"
#include "include/Cactus.h"
#include "include/SucculentCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* CactusFactory::buildPlant() const {
    static int cactusCounter = 1;
    std::string plantId = "CACTUS_" + std::to_string(cactusCounter++);
    CareStrategy* careStrategy = new SucculentCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Cactus* plant = new Cactus(plantId, careStrategy, initialState, "Columnar", "Saguaro");
    
    CareScheduler* scheduler = getScheduler();
    
    
    new WaterObserver(scheduler, plant);
    new FertilizeObserver(scheduler, plant);
    new SunlightObserver(scheduler, plant);
    
    return plant;
}