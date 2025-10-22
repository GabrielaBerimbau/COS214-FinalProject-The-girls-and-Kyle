#ifndef FLOOR_MANAGER_H
#define FLOOR_MANAGER_H

#include "StaffMembers.h"

class FloorManager : public StaffMembers{
    public:
        FloorManager();
        virtual ~FloorManager();
        virtual void handleRequest();
};

#endif //FLOOR_MANAGER_H