#ifndef SEEDLINGSTATE_H
#define SEEDLINGSTATE_H

#include "PlantState.h"
#include <string>

class Plant;

/**
 * @class SeedlingState
 * @brief Concrete state representing the seedling stage of a plant's lifecycle
 * 
 * This class implements the State pattern for plants in their earliest growth stage.
 * Seedlings are young, vulnerable plants that require frequent care and monitoring.
 * 
 * Characteristics of seedling state:
 * - Requires frequent watering (alerts when water level < 40%)
 * - Vulnerable to poor health conditions
 * - Transitions to GrowingState after approximately 7 days with decent health (≥50%)
 * - May transition to DeadState if health drops too low (<10%)
 */
class SeedlingState : public PlantState {
public:
    /**
     * @brief Constructor for SeedlingState
     * 
     * Prints a message indicating the plant has entered the seedling state
     */
    SeedlingState();
    
    /**
     * @brief Destructor for SeedlingState
     */
    ~SeedlingState();
    
    /**
     * @brief Handle seedling-specific behavior and state transitions
     * 
     * Manages the behavior of plants in the seedling stage:
     * - Checks if plant has died (health < 10%) → transitions to DeadState
     * - Checks if seedling is ready to grow (age ≥ 7 days, health ≥ 50%) → transitions to GrowingState
     * - Alerts if water level is critically low (< 40%)
     * 
     * @param plant Pointer to the plant in seedling state
     */
    void handleChange(Plant* plant) override;

    /**
     * @brief Get the name of this state
     * 
     * @return std::string Returns "Seedling"
     */
    std::string getStateName() override;
};

#endif