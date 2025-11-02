/**
 * @file MonsteraFactory.h
 * @author Megan Azmanov
 * @brief Declares the MonsteraFactory concrete class for creating monstera plants.
 * 
 * This class provides a concrete implementation of the OtherPlantFactory
 * for creating Monstera plant instances.
 */
#ifndef MONSTERA_FACTORY_H
#define MONSTERA_FACTORY_H

#include "OtherPlantFactory.h"

class CareScheduler;
class Plant;

/**
 * @class MonsteraFactory
 * @brief Concrete factory class for creating Monstera plants.
 * 
 * This class implements the Factory Method pattern to create
 * Monstera objects with optional care scheduling capabilities.
 */
class MonsteraFactory : public OtherPlantFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~MonsteraFactory() {}
    
    /**
     * @brief Creates a new Monstera plant instance.
     * 
     * Instantiates and returns a new Monstera object, 
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Monstera plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // MONSTERA_FACTORY_H
