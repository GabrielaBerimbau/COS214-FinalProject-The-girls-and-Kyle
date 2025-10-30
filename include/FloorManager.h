#ifndef FLOOR_MANAGER_H
#define FLOOR_MANAGER_H

#include "StaffMembers.h"
#include <string>

/**
 * @file FloorManager.h
 * @brief Mid-level handler in the staff chain (Chain of Responsibility)
 *
 * The FloorManager handles medium-complexity requests such as bulk orders or
 * special arrangements. If the request is outside its scope it escalates
 * to the next handler (e.g. NurseryOwner).
 *
 * Concrete implementations should focus on domain-specific logic and call
 * `request->markHandled()` when a request has been satisfied.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

/**
 * @class FloorManager
*/
class FloorManager: public StaffMembers{
    private:
        std::string name;
        std::string id;

    public:
        /**
         * @brief Constructor
         * @param med Pointer to mediator
         * @param staffName Name of floor manager
         * @param staffId ID of floor manager
         */
        FloorManager(NurseryMediator* med, std::string staffName, std::string staffId);
        
        virtual ~FloorManager();
        
        /**
         * @brief Handle request or escalate to next handler
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