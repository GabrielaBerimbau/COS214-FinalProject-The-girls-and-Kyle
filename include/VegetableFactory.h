/**
 * @file VegetableFactory.h
 * @author Megan Azmanov
 * @brief Declares the VegetableFactory abstract class for creating vegetable plants.
 * 
 * This class extends PlantFactory to specialize in vegetable creation while
 * remaining abstract to allow for specific vegetable type implementations.
 */
#ifndef VEGETABLE_FACTORY_H
#define VEGETABLE_FACTORY_H

#include "PlantFactory.h"

/**
 * @class VegetableFactory
 * @brief Abstract factory class for creating vegetable-type plants.
 * 
 * This class serves as an intermediate abstraction in the factory hierarchy,
 * grouping all vegetable-specific factories. concrete vegtablefactory plants 
 * will inherit from this class and provide specific implementations.
 */
class VegetableFactory : public PlantFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of derived classes.
     */
    virtual ~VegetableFactory() {}
    
    /**
     * @brief Creates a new vegetable plant instance.
     * 
     * This method remains pure virtual, concrete vegetable factories
     * must override this to create specific vegetable types.
     * 
     * @param scheduler pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const = 0;
};

#endif // VEGETABLE_FACTORY_H
