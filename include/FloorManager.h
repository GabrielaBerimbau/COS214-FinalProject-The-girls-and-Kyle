#ifndef FLOOR_MANAGER_H
#define FLOOR_MANAGER_H

#include "StaffMembers.h"
#include <string>

class FloorManager: public StaffMembers{
    private:
        std::string name;
        std::string id;

    public:
        FloorManager();
        FloorManager(std::string staffName, std::string staffId);
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