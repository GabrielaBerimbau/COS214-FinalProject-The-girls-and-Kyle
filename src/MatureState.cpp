#include "include/MatureState.h"
#include "include/FloweringState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>


MatureState::MatureState() {
    std::cout << "Plant entered Mature state - ready for sale!" << std::endl;
}


MatureState::~MatureState() {
}


void MatureState::handleChange(Plant* plant) {
   int age = plant->getAge();
    int health = plant->getHealthLevel();
    
    // Check if plant has died due to poor health
    if (health < 10) {
        std::cout << "Mature plant " << plant->getID() << " has died due to poor health." << std::endl;
        plant->setState(new DeadState());
        //delete this;
        return;
    }
    
    // Check if plant is ready to transition to Flowering state
    if (age >= 35 && health >= 80) {
        std::cout << "Mature plant " << plant->getID() << " is starting to flower... transitioning to Flowering state." << std::endl;
        plant->setState(new FloweringState());
        //delete this;
        return;
    }
    
    // Mark plant as ready for sale
    plant->setReadyForSale(true);
    
    // Alert if water level is low
    if (plant->getWaterLevel() < 20) {
        std::cout << "Mature plant " << plant->getID() << " needs water." << std::endl;
    }
}


std::string MatureState::getStateName() {
    return "Mature";
}