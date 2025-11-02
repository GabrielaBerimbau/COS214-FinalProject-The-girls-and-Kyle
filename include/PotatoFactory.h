/**
 * @file PotatoFactory.h
 * @author Megan Azmanov
 * @brief Declares the PotatoFactory concrete class for creating potato plants.
 * 
 * This class provides a concrete implementation of the VegetableFactory
 * for creating Potato plant instances.
 */
#ifndef POTATO_FACTORY_H
#define POTATO_FACTORY_H

#include "VegetableFactory.h"

/**
 * @class PotatoFactory
 * @brief Concrete factory class for creating Potato plants.
 * 
 * This class implements the Factory Method pattern to create
 * Potato plant objects with  care scheduling capabilities.
 */
class PotatoFactory : public VegetableFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~PotatoFactory() {}
    
    /**
     * @brief Creates a new Potato plant instance.
     * 
     * Instantiates and returns a new Potato object, 
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Potato plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // POTATO_FACTORY_H
