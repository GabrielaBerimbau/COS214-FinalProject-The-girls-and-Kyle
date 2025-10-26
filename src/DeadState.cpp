#include "DeadState.h"
#include "Plant.h"
#include <iostream>

DeadState::DeadState() {
    std::cout << "Plant has died." << std::endl;
}

DeadState::~DeadState() {
}

void DeadState::handleChange(Plant* plant) {
    plant->setReadyForSale(false);
    
    plant->setPrice(0.0);
    
    std::cout << "Plant " << plant->getID() << " is dead and should be removed from inventory." << std::endl;
}

std::string DeadState::getStateName() {
    return "Dead";
}