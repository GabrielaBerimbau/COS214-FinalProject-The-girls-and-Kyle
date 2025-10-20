#ifndef STAFF_MEMBERS_H
#define STAFF_MEMBERS_H

#include "Request.h"

class StaffMembers {
    public:
        StaffMembers();
        virtual ~StaffMembers();
        virtual void handleRequest();
};

#endif //STAFF_MEMBERS_H