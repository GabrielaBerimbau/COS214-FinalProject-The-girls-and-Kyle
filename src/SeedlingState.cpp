#include "include/SeedlingState.h"
#include "include/GrowingState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

SeedlingState::SeedlingState() {
    std::cout << "Plant entered Seedling state." << std::endl;
}

SeedlingState::~SeedlingState() {
}

void SeedlingState::handleChange(Plant* plant) {
    int age = plant->getAge();
    int health = plant->getHealthLevel();
    
    if (health < 10) {
        std::cout << "Seedling " << plant->getID() << " has died due to poor health." << std::endl;
        plant->setState(new DeadState());
        //delete this;
        return;
    }
    
    if (age >= 7 && health >= 50) {
        std::cout << "Seedling " << plant->getID() << " is growing... transitioning to Growing state." << std::endl;
        plant->setState(new GrowingState());
        //delete this;
        return;
    }
    
    if (plant->getWaterLevel() < 40) {
        std::cout << "Seedling " << plant->getID() << " needs water urgently!" << std::endl;
    }
}

std::string SeedlingState::getStateName() {
    return "Seedling";
}