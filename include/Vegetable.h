/**
 * @file Vegetable.h
 * @author Kyle McCalgan
 * @brief Declares the Vegetable class, representing vegetable plants in the nursery.
 *
 * This class provides a concrete implementation of the Plant base class
 * for vegetable-type plants with specific growth and harvest characteristics.
 */
#ifndef VEGETABLE_H
#define VEGETABLE_H

#include "Plant.h"

/**
 * @class Vegetable
 * @brief Represents a vegetable plant in the nursery system.
 *
 * This class extends the Plant base class to represent vegetables with
 * specific growth patterns and harvest requirements.
 */
class Vegetable : public Plant {
public:
    /**
     * @brief Constructs a new Vegetable object.
     * @param name The name of the vegetable.
     * @param id Unique identifier for the vegetable.
     * @param careStrategy Pointer to the care strategy for this vegetable.
     * @param initialState Pointer to the initial state of the vegetable.
     */
    Vegetable(const std::string& name, const std::string& id,
              CareStrategy* careStrategy, PlantState* initialState);

    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of vegetable resources.
     */
    virtual ~Vegetable();

    /**
     * @brief Performs daily update operations specific to vegetables.
     * Overrides the base Plant class implementation.
     */
    void dailyUpdate() override;

    /**
     * @brief Converts vegetable information to a string representation.
     * @return String containing vegetable-specific details.
     */
    std::string toString() const override;
};

#endif