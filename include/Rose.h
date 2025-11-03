/**
 * @file Rose.h
 * @author Kyle McCalgan
 * @brief Declares the Rose class, a specific type of flower.
 *
 * This class represents a rose plant with specific color and type attributes,
 * extending the Flower base class.
 */
#ifndef ROSE_H
#define ROSE_H

#include "Flower.h"

/**
 * @class Rose
 * @brief Represents a rose plant in the nursery system.
 *
 * This class extends the Flower class to represent roses with
 * specific color and type varieties.
 */
class Rose : public Flower {
public:
    /**
     * @brief Constructs a new Rose object.
     * @param id Unique identifier for the rose.
     * @param careStrategy Pointer to the care strategy for this rose.
     * @param initialState Pointer to the initial state of the rose.
     * @param roseColor The color of the rose.
     * @param type The type/variety of rose (defaults to "Hybrid Tea").
     */
    Rose(const std::string& id, CareStrategy* careStrategy,
         PlantState* initialState, const std::string& roseColor,
         const std::string& type = "Hybrid Tea");

    /**
     * @brief Destructor for Rose.
     * Cleans up rose-specific resources.
     */
    ~Rose();

    /**
     * @brief Converts rose information to a string representation.
     * @return String containing rose-specific details including color and type.
     */
    std::string toString() const override;
};

#endif