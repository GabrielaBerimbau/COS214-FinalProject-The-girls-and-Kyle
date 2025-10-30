/**
 * @file WaterPlantCommand.h
 * @author Megan Azmanov
 * @brief Declares the WaterPlantCommand concrete class for watering plant operations.
 * 
 * This class provides a concrete implementation of the Command interface
 * for encapsulating the action of watering a plant.
 */
#ifndef WATER_PLANT_COMMAND_H
#define WATER_PLANT_COMMAND_H

#include "Command.h"

class Plant;

/**
 * @class WaterPlantCommand
 * @brief Concrete command for executing plant watering operations.
 * 
 * This class implements the Command Pattern to encapsulate the request
 * to water a specific plant. It maintains a reference to the target plant
 * but does not own it, allowing the command to be queued and executed later
 * by the CareScheduler.
 */
class WaterPlantCommand : public Command {
public:
    /**
     * @brief Constructs a water plant command for a specific target plant.
     * 
     * @param target Pointer to the Plant object to be watered. The command does not take ownership.
     */
    explicit WaterPlantCommand(Plant* target);
    
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup. Note that the target plant is not deleted.
     */
    virtual ~WaterPlantCommand() {}

    /**
     * @brief Executes the watering action on the target plant.
     * 
     * Invokes the appropriate watering method on the target plant,
     * updating its water level or state accordingly.
     */
    virtual void execute();

private:
    Plant* target_; ///< Pointer to the target plant (not owned by this command)
};

#endif // WATER_PLANT_COMMAND_H