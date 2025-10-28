#ifndef NURSERY_OWNER_H
#define NURSERY_OWNER_H

#include "StaffMembers.h"
#include <string>

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