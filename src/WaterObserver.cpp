#include "include/WaterObserver.h"
#include "include/Plant.h"
#include "include/CareScheduler.h"
#include "include/WaterPlantCommand.h"

WaterObserver::WaterObserver(CareScheduler* scheduler, Plant* plant)
    : scheduler_(scheduler), plant_(plant) {
    if (plant_ != nullptr) {
        plant_->attach(this);
    }
}

WaterObserver::~WaterObserver() {
    if (plant_ != nullptr) {
        plant_->detach(this);
    }
}

void WaterObserver::update(Plant* plant) {
    if (plant->getWaterLevel() < 30) {
        Command* cmd = new WaterPlantCommand(plant);
        scheduler_->addTask(cmd);
    }
}
