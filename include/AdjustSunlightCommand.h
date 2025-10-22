#ifndef ADJUST_SUNLIGHT_COMMAND_H
#define ADJUST_SUNLIGHT_COMMAND_H

#include "Command.h"

class Plant;

class AdjustSunlightCommand : public Command {
public:
    explicit AdjustSunlightCommand(Plant* target);
    virtual ~AdjustSunlightCommand() {}

    virtual void execute();

private:
    Plant* target_; // not owned
};

#endif // ADJUST_SUNLIGHT_COMMAND_H