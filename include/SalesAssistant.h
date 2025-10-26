#ifndef SALES_ASSISTANT_H
#define SALES_ASSISTANT_H

#include "StaffMembers.h"
#include "Plant.h"
#include "CareScheduler.h"
#include <string>

class SalesAssistant: public StaffMembers{
    private:
        CareScheduler* scheduler;
        std::string name;
        std::string id;

    public:
        SalesAssistant();
        SalesAssistant(std::string staffName, std::string staffId);
        virtual ~SalesAssistant();
        
        /**
         * @brief Handle request or escalate to the next handler
         * @param request Request to handle
         */
        void handleRequest(Request* request) override;
        
        /**
         * @brief Default handle request
         */
        void handleRequest() override;
        
        /**
         * @brief Process customer plant request
         * @param plantName Name of plant requested
         */
        void customerRequestsPlant(std::string plantName);
        
        /**
         * @brief Find requested plant on sales floor
         * @param plantName Plant to find
         * @return Pointer to plant if found
         */
        Plant* findRequestedPlant(std::string plantName);
        
        /**
         * @brief Get staff name
         * @return Staff name
         */
        std::string getName() const;
        
        /**
         * @brief Get staff ID
         * @return Staff ID
         */
        std::string getId() const;
        
        /**
         * @brief Run all scheduled care tasks
         */
        void runCareScheduler();
};

#endif