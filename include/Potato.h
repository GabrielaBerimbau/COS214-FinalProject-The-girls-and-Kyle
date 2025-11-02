/**
 * @file Potato.h
 * @author Kyle McCalgan
 * @brief Declares the Potato class, a specific type of vegetable.
 *
 * This class represents a potato plant with specific variety and skin color,
 * extending the Vegetable base class.
 */
#ifndef POTATO_H
#define POTATO_H

#include "Vegetable.h"

/**
 * @class Potato
 * @brief Represents a potato plant in the nursery system.
 *
 * This class extends the Vegetable class to represent potatoes with
 * specific variety and skin color characteristics.
 */
class Potato : public Vegetable {
public:
    /**
     * @brief Constructs a new Potato object.
     * @param id Unique identifier for the potato.
     * @param careStrategy Pointer to the care strategy for this potato.
     * @param initialState Pointer to the initial state of the potato.
     * @param variety The variety of potato (defaults to "Russet").
     * @param skin The skin color of the potato (defaults to "brown").
     */
    Potato(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety = "Russet",
           const std::string& skin = "brown");

    /**
     * @brief Destructor for Potato.
     * Cleans up potato-specific resources.
     */
    ~Potato();

    /**
     * @brief Converts potato information to a string representation.
     * @return String containing potato-specific details including variety and skin color.
     */
    std::string toString() const override;
};

#endif