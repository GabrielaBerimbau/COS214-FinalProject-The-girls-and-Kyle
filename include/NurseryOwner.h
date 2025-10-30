#ifndef NURSERY_OWNER_H
#define NURSERY_OWNER_H

#include "StaffMembers.h"
#include <string>

/**
 * @file NurseryOwner.h
 * @brief Final handler in the staff chain (Chain of Responsibility)
 *
 * The NurseryOwner handles the most complex/sensitive requests including:
 * - Customer complaints and refund requests
 * - Legal/urgent issues requiring executive decision
 * - Final escalation point for any unresolved issues
 *
 * As the final handler in the chain, NurseryOwner must be prepared to
 * make final decisions on any requests that reach it.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

class NurseryOwner: public StaffMembers{
    private:
        std::string name;
        std::string id;

    public:
        /**
         * @brief Constructor
         * @param med Pointer to mediator
         * @param staffName Name of nursery owner
         * @param staffId ID of nursery owner
         */
        NurseryOwner(NurseryMediator* med, std::string staffName, std::string staffId);
        
        virtual ~NurseryOwner();
        
        /**
         * @brief Handle final level requests (no escalation from here)
         * @param request Request to handle
         */
        void handleRequest(Request* request) override;
        
        /**
         * @brief Default handle request
         */
        void handleRequest() override;
        
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
};

#endif