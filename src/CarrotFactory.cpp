#include "include/CarrotFactory.h"
#include "include/Carrot.h"
#include "include/VegetableCareStrategy.h"
#include "include/SeedlingState.h"
#include "include/WaterObserver.h"
#include "include/FertilizeObserver.h"
#include "include/SunlightObserver.h"
#include "include/CareScheduler.h"

Plant* CarrotFactory::buildPlant(CareScheduler* scheduler) const {
    static int CarrotCounter = 1;
    std::string plantId = "Carrot_" + std::to_string(CarrotCounter++);
    CareStrategy* careStrategy = new VegetableCareStrategy();
    PlantState* initialState = new SeedlingState();
    
    Carrot* plant = new Carrot(plantId, careStrategy, initialState, "Russet", "Brown");
    
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