#ifndef WATER_PLANT_COMMAND_H
#define WATER_PLANT_COMMAND_H

#include "Command.h"

class Plant;

class WaterPlantCommand : public Command {
public:
    explicit WaterPlantCommand(Plant* target);
    virtual ~WaterPlantCommand() {}

    virtual void execute();

private:
    Plant* target_; // not owned
};

#endif // WATER_PLANT_COMMAND_H