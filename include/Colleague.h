#ifndef COLLEAGUE_H
#define COLLEAGUE_H

#include <string>

/**
 * @class Colleague
 * @brief Abstract base class for all entities that communicate through the mediator
 */
class Colleague{
    protected:
        NurseryMediator* mediator;

    public:
        /**
         * @brief Constructor
         * @param m Pointer to the mediator
         */
        Colleague(NurseryMediator* m);

        /**
         * @brief Virtual destructor
         */
        virtual ~Colleague();

        /**
         * @brief Set or change the mediator
         * @param m Pointer to the new mediator
         */
        void setMediator(NurseryMediator* m);

        /**
         * @brief Get the current mediator
         * @return Pointer to the mediator
         */
        NurseryMediator* getMediator()const;
};

#endif