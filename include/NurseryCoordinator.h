#ifndef NURSERYCOORDINATOR_H
#define NURSERYCOORDINATOR_H

#include "NurseryMediator.h"

/**
 * @class NurseryCoordinator
 * @brief Extended mediator with additional coordination capabilities
 */
class NurseryCoordinator: public NurseryMediator{
    public:
        /**
         * @brief Constructor
         */
        NurseryCoordinator();

        /**
         * @brief Destructor
         */
        ~NurseryCoordinator();
};

#endif