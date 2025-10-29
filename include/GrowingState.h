#ifndef GROWINGSTATE_H
#define GROWINGSTATE_H

#include "PlantState.h"

class Plant;

/**
 * @class GrowingState
 * @brief Concrete state representing the active growth stage of a plant's lifecycle
 * 
 * This class implements the State pattern for plants in their growth stage.
 * Growing plants are in an active development phase, requiring regular care
 * and maintenance to reach maturity.
 * 
 * Characteristics of growing state:
 * - Active growth phase requiring regular care
 * - Monitors water levels (alerts when < 30%)
 * - Monitors nutrient levels (alerts when < 25%)
 * - Transitions to MatureState after approximately 20 days with good health (≥60%)
 * - May transition to DeadState if health drops critically low (<10%)
 */
class GrowingState : public PlantState {
public:
    /**
     * @brief Constructor for GrowingState
     * 
     * Prints a message indicating the plant has entered the growing state
     */
    GrowingState();
    
    /**
     * @brief Destructor for GrowingState
     */
    ~GrowingState();
    
    /**
     * @brief Handle growing-specific behavior and state transitions
     * 
     * Manages the behavior of plants in the growing stage:
     * - Checks if plant has died (health < 10%) → transitions to DeadState
     * - Checks if plant is ready to mature (age ≥ 20 days, health ≥ 60%) → transitions to MatureState
     * - Alerts if water level is low (< 30%)
     * - Alerts if nutrient level is low (< 25%)
     * 
     * @param plant Pointer to the plant in growing state
     */
    void handleChange(Plant* plant) override;

    /**
     * @brief Get the name of this state
     * 
     * @return std::string Returns "Growing"
     */
    std::string getStateName() override;
};

#endif