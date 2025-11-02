/**
 * @file CactusFactory.h
 * @author Megan Azmanov
 * @brief Declares the CactusFactory concrete class for creating cactus plants.
 * 
 * This class provides a concrete implementation of the SucculentFactory
 * for creating Cactus plant instances.
 */
#ifndef CACTUS_FACTORY_H
#define CACTUS_FACTORY_H

#include "SucculentFactory.h"

/**
 * @class CactusFactory
 * @brief Concrete factory class for creating Cactus plants.
 * 
 * This class implements the Factory Method pattern to create
 * Cactus plant objects with  care scheduling capabilities.
 */
class CactusFactory : public SucculantFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~CactusFactory() {}
    
    /**
     * @brief Creates a new Cactus plant instance.
     * 
     * Instantiates and returns a new Cactus object, 
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Cactus plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // CACTUS_FACTORY_H
