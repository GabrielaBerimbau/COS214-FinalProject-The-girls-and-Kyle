#ifndef SALES_ASSISTANT_H
#define SALES_ASSISTANT_H

#include "StaffMembers.h"
#include "Plant.h"
#include "CareScheduler.h"
#include <string>

/**
 * @file SalesAssistant.h
 * @brief Sales assistant handler (entry-level in Chain of Responsibility)
 *
 * The SalesAssistant handles low-complexity customer requests (e.g. simple
 * plant requests) and uses the `NurseryMediator` to locate and transfer
 * plants to customers. If a request is beyond its responsibility it should
 * escalate to the `nextHandler`.
 *
 * Implementations should call `mediator->staffAddPlantToCustomerCart(...)`
 * to keep transfer logic centralised in the mediator.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

/**
 * @class SalesAssistant
*/
class SalesAssistant: public StaffMembers{
    private:
        CareScheduler* scheduler;

    public:
        /**
         * @brief Constructor
         * @param med Pointer to mediator
         * @param staffName Name of sales assistant
         * @param staffId ID of sales assistant
         */
        SalesAssistant(NurseryMediator* med, std::string staffName, std::string staffId);

        /**
         * @brief Virtual destructor
         * Ensures proper cleanup of sales assistant resources
         */
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
         * @brief Run all scheduled care tasks
         */
        void runCareScheduler();
};

#endif