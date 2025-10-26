#include "AdjustSunlightCommand.h"
#include "Plant.h"
#include "CareStrategy.h"
#include <iostream>

AdjustSunlightCommand::AdjustSunlightCommand(Plant* target) 
    : target_(target) {
}

void AdjustSunlightCommand::execute() {
    if (target_ != nullptr && target_->getStrategy() != nullptr) {
        target_->getStrategy()->adjustSunlight(target_);
    }
}

