#include "include/FloweringState.h"
#include "include/MatureState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @brief Constructor for FloweringState
 * 
 * Initializes the flowering state and outputs a message indicating
 * the plant has entered this premium quality stage.
 */
FloweringState::FloweringState() {
    std::cout << "Plant entered Flowering state!" << std::endl;
}

/**
 * @brief Destructor for FloweringState
 * 
 * Cleans up the flowering state object. No special cleanup required.
 */
FloweringState::~FloweringState() {
}

/**
 * @brief Handle flowering-specific behavior and state transitions
 * 
 * Manages the plant's behavior during the flowering stage by:
 * 1. Checking if the plant has died due to poor health (health < 10%)
 *    - If so, transitions to DeadState
 * 2. Marking the plant as ready for sale
 * 3. Applying premium pricing (1.5x multiplier) if price is below threshold
 * 4. Checking if flowering period has ended (age ≥ 50 days)
 *    - If so, transitions back to MatureState
 * 5. Monitoring water levels and alerting if low (< 30%)
 * 6. Monitoring nutrient levels and alerting if low (< 30%)
 * 7. Monitoring sunlight exposure and alerting if insufficient (< 50%)
 * 
 * Flowering plants are at their most attractive and valuable stage,
 * requiring careful maintenance to preserve bloom quality.
 * 
 * @param plant Pointer to the plant whose state is being managed
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
 * 
 * Returns a string identifier for the flowering state, used for
 * display purposes and state identification throughout the system.
 * 
 * @return std::string Returns "Flowering"
 */
std::string FloweringState::getStateName() {
    return "Flowering";
}