/**
 * @file Command.h
 * @author Megan Azmanov
 * @brief Declares the Command interface, serving as the base for Command pattern implementations.
 * 
 * This abstract class defines the essential interface for all concrete command types in the system.
 * It enforces implementation of the execute method for encapsulating plant care actions.
 */
#ifndef COMMAND_H
#define COMMAND_H

/**
 * @class Command
 * @brief Abstract base class defining the interface for commands.
 * 
 * This class forms the foundation of the Command Pattern implementation.
 * It provides a pure virtual execute function that encapsulates requests as objects,
 * allowing for parameterization of clients with different requests, queuing of requests,
 * and logging of operations.
 */
class Command {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of derived classes.
     */
    virtual ~Command() {}
    
    /**
     * @brief Executes the encapsulated command action.
     * 
     * This method must be implemented by all subclasses to perform
     * the specific action associated with the command.
     */
    virtual void execute() = 0;
};

#endif // COMMAND_H
