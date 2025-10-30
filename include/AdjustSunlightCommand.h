/**
 * @file AdjustSunlightCommand.h
 * @author Megan Azmanov
 * @brief Declares the AdjustSunlightCommand concrete class for sunlight adjustment operations.
 * 
 * This class provides a concrete implementation of the Command interface
 * for encapsulating the action of adjusting a plant's sunlight exposure.
 */
#ifndef ADJUST_SUNLIGHT_COMMAND_H
#define ADJUST_SUNLIGHT_COMMAND_H

#include "Command.h"

class Plant;

/**
 * @class AdjustSunlightCommand
 * @brief Concrete command for executing sunlight adjustment operations.
 * 
 * This class implements the Command Pattern to encapsulate the request
 * to adjust sunlight exposure for a specific plant. It maintains a reference
 * to the target plant but does not own it, allowing the command to be queued
 * and executed later by the CareScheduler.
 */
class AdjustSunlightCommand : public Command {
public:
    /**
     * @brief Constructs a sunlight adjustment command for a specific target plant.
     * 
     * @param target Pointer to the Plant object whose sunlight exposure will be adjusted. 
     *               The command does not take ownership.
     */
    explicit AdjustSunlightCommand(Plant* target);
    
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup. Note that the target plant is not deleted.
     */
    virtual ~AdjustSunlightCommand() {}

    /**
     * @brief Executes the sunlight adjustment action on the target plant.
     * 
     * Invokes the appropriate sunlight adjustment method on the target plant,
     * updating its sunlight level or exposure state accordingly.
     */
    virtual void execute();

private:
    Plant* target_;
};

#endif // ADJUST_SUNLIGHT_COMMAND_H
