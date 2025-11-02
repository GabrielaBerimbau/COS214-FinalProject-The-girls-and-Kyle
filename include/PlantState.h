#ifndef PLANTSTATE_H
#define PLANTSTATE_H

#include <string>

class Plant;

/**
 * @class PlantState
 * @brief Abstract base class representing a state in the plant lifecycle (state design pattern)
 * @author Sofia Finlayson
 * @date 2025-10-26
 * 
 * This class is part of the State design pattern implementation for managing
 * plant lifecycle stages. Each concrete state (SeedlingState, GrowingState, MatureState, 
 * FloweringState, DeadState) inherits from this class and implements state-specific behavior.
 * 
 * The State pattern allows a plant to alter its behavior when its internal state
 * changes, appearing as if the plant changed its class.
 */
class PlantState {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~PlantState() {}
    
    /**
     * @brief Handle state-specific behavior and transitions
     * 
     * This pure virtual method must be implemented by concrete state classes.
     * It defines how the plant behaves in the current state and determines
     * when to transition to other states based on age, health, and conditions.
     * 
     * @param plant Pointer to the plant whose state is being managed
     */
    virtual void handleChange(Plant* plant) = 0;

    /**
     * @brief Get the name of the current state
     * 
     * This pure virtual method must be implemented by concrete state classes
     * to return a string representation of the state name (e.g., "Seedling",
     * "Growing", "Mature", "Flowering", "Dead").
     * 
     * @return std::string The name of the current state
     */
    virtual std::string getStateName() = 0;
    
protected:
    /**
     * @brief Protected constructor to prevent direct instantiation
     * 
     * Only concrete state classes can be instantiated
     */
    PlantState() {}
};

#endif