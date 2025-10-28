// include/StaffMembers.h
#ifndef STAFF_MEMBERS_H
#define STAFF_MEMBERS_H

#include "Request.h"
#include "Person.h"

class StaffMembers: public Person{
    protected:
        StaffMembers* nextHandler;

    public:
        /**
         * @brief Constructor
         * @param med Pointer to mediator
         * @param staffName Name of staff member
         * @param staffId ID of staff member
         */
        StaffMembers(NurseryMediator* med, std::string staffName, std::string staffId);
        
        virtual ~StaffMembers();
        
        /**
         * @brief Set next handler in chain
         * @param next Pointer to next handler
         */
        void setNext(StaffMembers* next);
        
        /**
         * @brief Handle request or pass to next handler
         * @param request Request to handle
         */
        virtual void handleRequest(Request* request) = 0;
        
        /**
         * @brief Default handle request (for compatibility)
         */
        virtual void handleRequest();
};

#endif