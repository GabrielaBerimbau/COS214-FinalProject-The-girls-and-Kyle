#ifndef CARE_SCHEDULER_H
#define CARE_SCHEDULER_H

#include <vector>

class Command;

/**
 * Owns the queued commands (composition in UML).
 * It deletes any commands it still owns in the destructor.
 */
class CareScheduler {
public:
    CareScheduler();
    ~CareScheduler();                   // deletes queued commands

    void addTask(Command* cmd);         // takes ownership
    void runNext();                     // executes+deletes one
    void runAll();                      // executes+deletes all
    bool empty() const;

private:
    std::vector<Command*> queue_;
};

#endif // CARE_SCHEDULER_H