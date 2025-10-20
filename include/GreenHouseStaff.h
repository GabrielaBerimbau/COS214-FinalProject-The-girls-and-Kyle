#ifndef GREEN_HOUSE_STAFF_H
#define GREEN_HOUSE_STAFF_H

#include "StaffMembers.h"

class GreenHouseStaff : public StaffMembers{
    public:
        GreenHouseStaff();
        virtual ~GreenHouseStaff();
        virtual void handleRequest();
};

#endif //GREEN_HOUSE_STAFF_H