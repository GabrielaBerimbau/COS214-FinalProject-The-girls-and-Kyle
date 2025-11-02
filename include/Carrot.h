/**
 * @file Carrot.h
 * @author Kyle McCalgan
 * @brief Declares the Carrot class, a specific type of vegetable.
 *
 * This class represents a carrot plant with specific variety and skin color,
 * extending the Vegetable base class.
 */
#ifndef CARROT_H
#define CARROT_H

#include "Vegetable.h"

/**
 * @class Carrot
 * @brief Represents a carrot plant in the nursery system.
 *
 * This class extends the Vegetable class to represent carrots with
 * specific variety and skin color characteristics.
 */
class Carrot : public Vegetable {
public:
    /**
     * @brief Constructs a new Carrot object.
     * @param id Unique identifier for the carrot.
     * @param careStrategy Pointer to the care strategy for this carrot.
     * @param initialState Pointer to the initial state of the carrot.
     * @param variety The variety of carrot (defaults to "Imperator").
     * @param skin The skin color of the carrot (defaults to "orange").
     */
    Carrot(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety = "Imperator",
           const std::string& skin = "orange");

    /**
     * @brief Destructor for Carrot.
     * Cleans up carrot-specific resources.
     */
    ~Carrot();

    /**
     * @brief Converts carrot information to a string representation.
     * @return String containing carrot-specific details including variety and skin color.
     */
    std::string toString() const override;
};

#endif