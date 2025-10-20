#ifndef NURSERY_OWNER_H
#define NURSERY_OWNER_H

#include "StaffMembers.h"

class NurseryOwner : public StaffMembers{
    public:
        NurseryOwner();
        virtual ~NurseryOwner();
        virtual void handleRequest();
};

#endif //NURSERY_OWNER_H