/**
 * @file SucculentFactory.h
 * @author Megan Azmanov
 * @brief Declares the SucculentFactory abstract class for creating succulent plants.
 * 
 * This class extends PlantFactory to specialize in succulent creation
 * It remains abstract to allow for specific succulent implementations.
 */

#ifndef SUCCULANT_FACTORY_H
#define SUCCULANT_FACTORY_H

#include "PlantFactory.h"

class SucculantFactory : public PlantFactory {
public:

/**
     * @brief Virtual destructor.
     * Ensures proper cleanup of derived classes.
     */
    virtual ~SucculantFactory() {}


      /**
     * @brief Creates a new succulent plant instance.
     * 
     * This method remains pure virtual - concrete succulent factories
     * must override this to create specific succulent types.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const = 0;
};

#endif 