#include "include/SunlightObserver.h"
#include "include/Plant.h"
#include "include/CareScheduler.h"
#include "include/AdjustSunlightCommand.h"

SunlightObserver::SunlightObserver(CareScheduler* scheduler, Plant* plant)
    : scheduler_(scheduler), plant_(plant) {
    if (plant_ != nullptr) {
        plant_->attach(this);
    }
}

SunlightObserver::~SunlightObserver() {
    if (plant_ != nullptr) {
        plant_->detach(this);
    }
}

void SunlightObserver::update(Plant* plant) {
    if (plant->getSunlightExposure() < 40) {
        Command* cmd = new AdjustSunlightCommand(plant);
        scheduler_->addTask(cmd);
    }
}
