#ifndef FERTILIZE_PLANT_COMMAND_H
#define FERTILIZE_PLANT_COMMAND_H

#include "Command.h"

class Plant;

class FertilizePlantCommand : public Command {
public:
    explicit FertilizePlantCommand(Plant* target);
    virtual ~FertilizePlantCommand() {}

    virtual void execute();

private:
    Plant* target_; // not owned
};

#endif // FERTILIZE_PLANT_COMMAND_H