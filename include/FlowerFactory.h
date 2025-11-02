/**
 * @file FlowerFactory.h
 * @author Megan Azmanov
 * @brief Declares the FlowerFactory abstract class for creating flower plants.
 * 
 * This class extends PlantFactory to specialize in flower creation while
 * remaining abstract to allow for specific flower type implementations.
 */
#ifndef FLOWER_FACTORY_H
#define FLOWER_FACTORY_H

#include "PlantFactory.h"

/**
 * @class FlowerFactory
 * @brief Abstract factory class for creating flower-type plants.
 * 
 * This class serves as an intermediate abstraction in the factory hierarchy,
 * grouping all flower-specific factories. Concrete flower factories
 * will inherit from this class and provide specific implementations.
 */
class FlowerFactory : public PlantFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of derived classes.
     */
    virtual ~FlowerFactory() {}
    
    /**
     * @brief Creates a new flower plant instance.
     * 
     * This method remains pure virtual, concrete flower factories
     * must override this to create specific flower types.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const = 0;
};

#endif // FLOWER_FACTORY_H
