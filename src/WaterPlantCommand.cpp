#include "include/WaterPlantCommand.h"
#include "include/Plant.h"
#include "include/CareStrategy.h"
#include <iostream>

WaterPlantCommand::WaterPlantCommand(Plant* target) 
    : target_(target) {
}

void WaterPlantCommand::execute() {
    if (target_ != nullptr && target_->getStrategy() != nullptr) {
        target_->getStrategy()->water(target_);
    }
}

