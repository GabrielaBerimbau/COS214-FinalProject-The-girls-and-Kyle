/**
 * @file Monstera.h
 * @author Kyle McCalgan
 * @brief Declares the Monstera class, a specific type of decorative plant.
 *
 * This class represents a monstera plant with characteristic split leaves,
 * extending the OtherPlant base class.
 */
#ifndef MONSTERA_H
#define MONSTERA_H

#include "OtherPlant.h"
#include <string>

/**
 * @class Monstera
 * @brief Represents a monstera plant in the nursery system.
 *
 * This class extends the OtherPlant class to represent monstera plants,
 * known for their distinctive split leaves and tropical appearance.
 */
class Monstera : public OtherPlant {
public:
    /**
     * @brief Constructs a new Monstera object.
     * @param id Unique identifier for the monstera.
     * @param careStrategy Pointer to the care strategy for this monstera.
     * @param initialState Pointer to the initial state of the monstera.
     * @param initialLeaves The initial number of leaves (defaults to 3).
     */
    Monstera(const std::string& id, CareStrategy* careStrategy,
             PlantState* initialState, int initialLeaves = 3);

    /**
     * @brief Destructor for Monstera.
     * Cleans up monstera-specific resources.
     */
    ~Monstera();

    /**
     * @brief Converts monstera information to a string representation.
     * @return String containing monstera-specific details including leaf count.
     */
    std::string toString() const override;
};

#endif





        