// include/StaffMembers.h
#ifndef STAFF_MEMBERS_H
#define STAFF_MEMBERS_H

#include "Request.h"
#include "Person.h"

/**
 * @file StaffMembers.h
 * @brief Abstract base for staff chain of responsibility (Chain of Responsibility)
 *
 * StaffMembers provides the interface and shared behaviour for handlers in the
 * chain of responsibility used by the nursery staff (e.g. SalesAssistant,
 * FloorManager, NurseryOwner). Concrete handlers override `handleRequest`
 * to process requests of specific priority/complexity and may forward
 * unhandled requests to `nextHandler`.
 *
 * Usage notes:
 * - Create concrete subclasses for each staff role and implement request
 *   handling logic in `handleRequest(Request*)`.
 * - Chain handlers with `setNext()` so requests can escalate through the
 *   hierarchy.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

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