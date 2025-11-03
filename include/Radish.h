/**
 * @file Radish.h
 * @author Kyle McCalgan
 * @brief Declares the Radish class, a specific type of vegetable.
 *
 * This class represents a radish plant with specific variety and color,
 * extending the Vegetable base class.
 */
#ifndef RADISH_H
#define RADISH_H

#include "Vegetable.h"

/**
 * @class Radish
 * @brief Represents a radish plant in the nursery system.
 *
 * This class extends the Vegetable class to represent radishes with
 * specific variety and color characteristics.
 */
class Radish : public Vegetable {
public:
    /**
     * @brief Constructs a new Radish object.
     * @param id Unique identifier for the radish.
     * @param careStrategy Pointer to the care strategy for this radish.
     * @param initialState Pointer to the initial state of the radish.
     * @param variety The variety of radish (defaults to "Cherry Belle").
     * @param color The color of the radish (defaults to "red").
     */
    Radish(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety = "Cherry Belle",
           const std::string& color = "red");

    /**
     * @brief Destructor for Radish.
     * Cleans up radish-specific resources.
     */
    ~Radish();

    /**
     * @brief Converts radish information to a string representation.
     * @return String containing radish-specific details including variety and color.
     */
    std::string toString() const override;
};

#endif