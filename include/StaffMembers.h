// include/StaffMembers.h
#ifndef STAFF_MEMBERS_H
#define STAFF_MEMBERS_H

#include "Request.h"

class StaffMembers{
    protected:
        StaffMembers* nextHandler;

    public:
        StaffMembers();
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