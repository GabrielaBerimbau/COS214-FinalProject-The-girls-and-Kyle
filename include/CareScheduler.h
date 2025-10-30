/**
 * @file CareScheduler.h
 * @author Megan Azmanov
 * @brief Declares the CareScheduler class for managing and executing plant care commands.
 * 
 * This class serves as the invoker in the Command pattern, maintaining a queue
 * of commands and managing their execution lifecycle. It owns the queued commands
 * through composition and handles their proper cleanup.
 */
#ifndef CARE_SCHEDULER_H
#define CARE_SCHEDULER_H

#include <vector>

class Command;

/**
 * @class CareScheduler
 * @brief Manages a queue of plant care commands and controls their execution.
 * 
 * This class implements the invoker role in the Command Pattern.
 * It owns the queued commands through composition (as shown in UML) and is
 * responsible for their lifecycle management. Commands are deleted after execution
 * or when the scheduler is destroyed.
 * 
 * The scheduler provides flexibility in command execution, allowing commands to be
 * executed individually or in batch, supporting various scheduling strategies.
 */
class CareScheduler {
public:
    /**
     * @brief Default constructor.
     * Initializes an empty command queue.
     */
    CareScheduler();
    
    /**
     * @brief Destructor.
     * Deletes all remaining queued commands to prevent memory leaks.
     */
    ~CareScheduler();

    /**
     * @brief Adds a command to the execution queue.
     * 
     * Takes ownership of the provided command. The scheduler will be
     * responsible for deleting the command after execution or during cleanup.
     * 
     * @param cmd Pointer to the Command object to be queued. Ownership is transferred.
     */
    void addTask(Command* cmd);
    
    /**
     * @brief Executes and removes the next command in the queue.
     * 
     * Executes the first command in the queue in first come first server order, then deletes it.
     * If the queue is empty, this method has no effect.
     */
    void runNext();
    
    /**
     * @brief Executes and removes all commands in the queue.
     * 
     * Processes all queued commands in FIFO order, executing and deleting
     * each one until the queue is empty.
     */
    void runAll();
    
    /**
     * @brief Checks if the command queue is empty.
     * 
     * @return true if there are no commands in the queue, false otherwise.
     */
    bool empty() const;

private:
    std::vector<Command*> queue_;
};

#endif // CARE_SCHEDULER_H
