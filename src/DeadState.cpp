#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>


DeadState::DeadState() {
    std::cout << "Plant has died." << std::endl;
}


DeadState::~DeadState() {
}


void DeadState::handleChange(Plant* plant) {
    // Mark plant as not ready for sale
    plant->setReadyForSale(false);
    
    // Set price to 0 (dead plants have no value)
    plant->setPrice(0.0);
    
    // Notify that plant should be removed from inventory
    std::cout << "Plant " << plant->getID() << " is dead and should be removed from inventory." << std::endl;
}


std::string DeadState::getStateName() {
    return "Dead";
}