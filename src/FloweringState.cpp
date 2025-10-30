#include "include/FloweringState.h"
#include "include/MatureState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @author Sofia Finlayson
 * @date 2025-10-26
 * @brief Constructor for FloweringState
 */
FloweringState::FloweringState() {
    std::cout << "Plant entered Flowering state!" << std::endl;
}

/**
 * @brief Destructor for FloweringState
 * 
 */
FloweringState::~FloweringState() {
}

/**
 * @brief Handle flowering-specific behavior and state transitions
 */
void FloweringState::handleChange(Plant* plant) {
    int age = plant->getAge();
    int health = plant->getHealthLevel();
    
    // Check if plant has died due to poor health
    if (health < 10) {
        std::cout << "Flowering plant " << plant->getID() << " has died due to poor health." << std::endl;
        plant->setState(new DeadState());
        //delete this;
        return;
    }
    
    // Mark plant as ready for sale (premium quality)
    plant->setReadyForSale(true);
    
    // Apply premium pricing for flowering plants
    if (plant->getPrice() < 15.0) {
        plant->setPrice(plant->getPrice() * 1.5);
    }
    
    // Check if flowering period is over
    if (age >= 50) {
        std::cout << "Flowering plant " << plant->getID() << " has finished blooming... transitioning back to Mature state." << std::endl;
        plant->setState(new MatureState());
       // delete this;
        return;
    }
    
    // Alert if water level is low
    if (plant->getWaterLevel() < 30) {
        std::cout << "Flowering plant " << plant->getID() << " needs water to maintain blooms." << std::endl;
    }
    
    // Alert if nutrient level is low
    if (plant->getNutrientLevel() < 30) {
        std::cout << "Flowering plant " << plant->getID() << " needs fertilizer for blooms." << std::endl;
    }
    
    // Alert if sunlight exposure is insufficient
    if (plant->getSunlightExposure() < 50) {
        std::cout << "Flowering plant " << plant->getID() << " needs more sunlight." << std::endl;
    }
}

/**
 * @brief Get the name of this state
 */
std::string FloweringState::getStateName() {
    return "Flowering";
}