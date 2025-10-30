/**
 * @file FertilizePlantCommand.h
 * @author Megan Azmanov
 * @brief Declares the FertilizePlantCommand concrete class for fertilizing plant operations.
 * 
 * This class provides a concrete implementation of the Command interface
 * for encapsulating the action of fertilizing a plant.
 */
#ifndef FERTILIZE_PLANT_COMMAND_H
#define FERTILIZE_PLANT_COMMAND_H

#include "Command.h"

class Plant;

/**
 * @class FertilizePlantCommand
 * @brief Concrete command for executing plant fertilization operations.
 * 
 * This class implements the Command Pattern to encapsulate the request
 * to fertilize a specific plant. It maintains a reference to the target plant
 * but does not own it, allowing the command to be queued and executed later
 * by the CareScheduler.
 */
class FertilizePlantCommand : public Command {
public:
    /**
     * @brief Constructs a fertilize plant command for a specific target plant.
     * 
     * @param target Pointer to the Plant object to be fertilized. The command does not take ownership.
     */
    explicit FertilizePlantCommand(Plant* target);
    
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup. Note that the target plant is not deleted.
     */
    virtual ~FertilizePlantCommand() {}

    /**
     * @brief Executes the fertilization action on the target plant.
     * 
     * Invokes the appropriate fertilization method on the target plant,
     * updating its nutrient level or state accordingly.
     */
    virtual void execute();

private:
    Plant* target_; 
};

#endif // FERTILIZE_PLANT_COMMAND_H
