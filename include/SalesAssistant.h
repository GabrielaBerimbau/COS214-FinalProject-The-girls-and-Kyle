#ifndef SALES_ASSISTANT_H
#define SALES_ASSISTANT_H

#include "StaffMembers.h"
#include "Plant.h"
#include "CareScheduler.h" //check


#include <string>

class SalesAssistant : public StaffMembers{
    public:
        SalesAssistant();
        virtual ~SalesAssistant();
        virtual void handleRequest();
        void customerRequestsPlant(std::string plantName);
        Plant* findRequestedPlant(std::string plantName);

    private:
        CareScheduler
};

#endif //SALES_ASSISTANT_H