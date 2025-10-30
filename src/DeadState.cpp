#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @author Sofia Finlayson
 * @date 2025-10-26
 * @brief Constructor for DeadState
 */
DeadState::DeadState() {
    std::cout << "Plant has died." << std::endl;
}

/**
 * @brief Destructor for DeadState
 */
DeadState::~DeadState() {
}

/**
 * @brief Handle dead state behavior (terminal state with no transitions)
 */
void DeadState::handleChange(Plant* plant) {
    // Mark plant as not ready for sale
    plant->setReadyForSale(false);
    
    // Set price to 0 (dead plants have no value)
    plant->setPrice(0.0);
    
    // Notify that plant should be removed from inventory
    std::cout << "Plant " << plant->getID() << " is dead and should be removed from inventory." << std::endl;
}

/**
 * @brief Get the name of this state
 */
std::string DeadState::getStateName() {
    return "Dead";
}