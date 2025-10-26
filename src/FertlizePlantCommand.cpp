#include "include/FertilizePlantCommand.h"
#include "include/Plant.h"
#include "include/CareStrategy.h"
#include <iostream>

FertilizePlantCommand::FertilizePlantCommand(Plant* target) 
    : target_(target) {
}



void FertilizePlantCommand::execute() {
    if (target_ != nullptr && target_->getStrategy() != nullptr) {
        target_->getStrategy()->fertilize(target_);
    }
}