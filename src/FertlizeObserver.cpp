#include "include/FertilizeObserver.h"
#include "include/Plant.h"
#include "include/CareScheduler.h"
#include "include/FertilizePlantCommand.h"

FertilizeObserver::FertilizeObserver(CareScheduler* scheduler, Plant* plant)
    : scheduler_(scheduler), plant_(plant) {
    if (plant_ != nullptr) {
        plant_->attach(this);
    }
}

FertilizeObserver::~FertilizeObserver() {
    if (plant_ != nullptr) {
        plant_->detach(this);
    }
}

void FertilizeObserver::update(Plant* plant) {
    if (plant->getNutrientLevel() < 30) {
        Command* cmd = new FertilizePlantCommand(plant);
        scheduler_->addTask(cmd);
    }
}
