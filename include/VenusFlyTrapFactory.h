/**
 * @file VenusFlyTrapFactory.h
 * @author Megan Azmanov
 * @brief Declares the VenusFlyTrapFactory concrete class for creating Venus flytrap plants.
 * 
 * This class provides a concrete implementation of the OtherPlantFactory
 * for creating VenusFlytrap plant instances.
 */
#ifndef VENUS_FLY_TRAP_FACTORY_H
#define VENUS_FLY_TRAP_FACTORY_H

#include "OtherPlantFactory.h"

/**
 * @class VenusFlyTrapFactory
 * @brief Concrete factory class for creating Venus Flytrap plants.
 * 
 * This class implements the Factory Method pattern to create
 * Venus Flytrap objects.
 */
class VenusFlyTrapFactory : public OtherPlantFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~VenusFlyTrapFactory() {}
    
    /**
     * @brief Creates a new Venus Flytrap plant instance.
     * 
     * Instantiates and returns a new VenusFlytrap object,
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created VenusFlytrap plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // VENUS_FLY_TRAP_FACTORY_H
