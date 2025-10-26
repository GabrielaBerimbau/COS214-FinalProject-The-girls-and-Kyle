#include "include/GrowingState.h"
#include "include/MatureState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

GrowingState::GrowingState() {
    std::cout << "Plant entered Growing state." << std::endl;
}

GrowingState::~GrowingState() {
}

void GrowingState::handleChange(Plant* plant) {
    
    int age = plant->getAge();
    int health = plant->getHealthLevel();
    
    if (health < 10) {
        std::cout << "Growing plant " << plant->getID() << " has died due to poor health." << std::endl;
        plant->setState(new DeadState());
        delete this;
        return;
    }
    
    if (age >= 20 && health >= 60) {
        std::cout << "Plant " << plant->getID() << " is maturing... transitioning to Mature state." << std::endl;
        plant->setState(new MatureState());
        delete this;
        return;
    }
    
    if (plant->getWaterLevel() < 30) {
        std::cout << "Growing plant " << plant->getID() << " needs water." << std::endl;
    }
    
    if (plant->getNutrientLevel() < 25) {
        std::cout << "Growing plant " << plant->getID() << " needs fertilizer." << std::endl;
    }
}

std::string GrowingState::getStateName() {
    return "Growing";
}