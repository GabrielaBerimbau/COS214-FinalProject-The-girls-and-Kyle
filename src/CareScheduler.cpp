
#include "include/CareScheduler.h"
#include "include/Command.h"
#include <iostream>

CareScheduler::CareScheduler() {
    std::cout << "[CareScheduler] Scheduler created" << std::endl;
}

CareScheduler::~CareScheduler() {
    // Delete any remaining commands in queue
    for (Command* cmd : queue_) {
        delete cmd;
    }
    queue_.clear();
    std::cout << "[CareScheduler] Scheduler destroyed, cleaned up " 
              << queue_.size() << " remaining commands" << std::endl;
}

void CareScheduler::addTask(Command* cmd) {
    if (cmd != nullptr) {
        queue_.push_back(cmd);
        std::cout << "[CareScheduler] Task added to queue. Queue size: " 
                  << queue_.size() << std::endl;
    }
}

void CareScheduler::runNext() {
    if (!queue_.empty()) {
        Command* cmd = queue_.front();
        queue_.erase(queue_.begin());
        
        std::cout << "[CareScheduler] Executing next command..." << std::endl;
        cmd->execute();
        delete cmd;  // CareScheduler owns commands, so delete after execution
        
        std::cout << "[CareScheduler] Command executed and deleted. Remaining tasks: " 
                  << queue_.size() << std::endl;
    } else {
         std::cout << "[CareScheduler] No tasks in queue to execute" << std::endl;
    }
}

void CareScheduler::runAll() {
    if (queue_.empty()) {
         std::cout << "[CareScheduler] No tasks to execute" << std::endl;
        return;
    }
    
    size_t taskCount = queue_.size();
     std::cout << "[CareScheduler] Executing all " << taskCount << " queued tasks..." << std::endl;
    
    while (!queue_.empty()) {
        Command* cmd = queue_.front();
        queue_.erase(queue_.begin());
        
        cmd->execute();
        delete cmd;  //CSched owns commands
    }
    
     std::cout << "[CareScheduler] All " << taskCount << " tasks completed" << std::endl;
}



bool CareScheduler::empty() const {
    return queue_.empty();
}
