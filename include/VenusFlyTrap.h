/**
 * @file VenusFlyTrap.h
 * @author Kyle McCalgan
 * @brief Declares the VenusFlyTrap class, a carnivorous plant type.
 *
 * This class represents a Venus flytrap, a carnivorous plant with
 * snap-trap leaves, extending the OtherPlant base class.
 */
#ifndef VENUSFLYTRAP_H
#define VENUSFLYTRAP_H

#include "OtherPlant.h"

/**
 * @class VenusFlyTrap
 * @brief Represents a Venus flytrap plant in the nursery system.
 *
 * This class extends the OtherPlant class to represent Venus flytraps,
 * carnivorous plants with unique trap mechanisms for catching insects.
 */
class VenusFlyTrap : public OtherPlant {
public:
    /**
     * @brief Constructs a new VenusFlyTrap object.
     * @param id Unique identifier for the Venus flytrap.
     * @param careStrategy Pointer to the care strategy for this plant.
     * @param initialState Pointer to the initial state of the plant.
     * @param initialTraps The initial number of traps (defaults to 5).
     */
    VenusFlyTrap(const std::string& id, CareStrategy* careStrategy,
                 PlantState* initialState, int initialTraps = 5);

    /**
     * @brief Destructor for VenusFlyTrap.
     * Cleans up Venus flytrap-specific resources.
     */
    ~VenusFlyTrap();

    /**
     * @brief Converts Venus flytrap information to a string representation.
     * @return String containing plant details including trap count.
     */
    std::string toString() const override;
};

#endif