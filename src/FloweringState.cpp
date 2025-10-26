#include "FloweringState.h"
#include "MatureState.h"
#include "DeadState.h"
#include "Plant.h"
#include <iostream>

FloweringState::FloweringState() {
    std::cout << "Plant entered Flowering state!" << std::endl;
}

FloweringState::~FloweringState() {
}

void FloweringState::handleChange(Plant* plant) {
    int age = plant->getAge();
    int health = plant->getHealthLevel();
    
    if (health < 10) {
        std::cout << "Flowering plant " << plant->getID() << " has died due to poor health." << std::endl;
        plant->setState(new DeadState());
        delete this;
        return;
    }
    
    plant->setReadyForSale(true);
    
    if (plant->getPrice() < 15.0) {
        plant->setPrice(plant->getPrice() * 1.5);
    }
    if (age >= 50) {
        std::cout << "Flowering plant " << plant->getID() << " has finished blooming... transitioning back to Mature state." << std::endl;
        plant->setState(new MatureState());
        delete this;
        return;
    }
    
    if (plant->getWaterLevel() < 30) {
        std::cout << "Flowering plant " << plant->getID() << " needs water to maintain blooms." << std::endl;
    }
    
    if (plant->getNutrientLevel() < 30) {
        std::cout << "Flowering plant " << plant->getID() << " needs fertilizer for blooms." << std::endl;
    }
    
    if (plant->getSunlightExposure() < 50) {
        std::cout << "Flowering plant " << plant->getID() << " needs more sunlight." << std::endl;
    }
}

std::string FloweringState::getStateName() {
    return "Flowering";
}