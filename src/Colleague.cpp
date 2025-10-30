/**
 * @file Colleague.cpp
 * @brief Implementation of base Colleague class for Mediator pattern
 * 
 * Provides basic mediator management functionality. See Colleague.h for
 * the complete interface documentation.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

#include "../include/Colleague.h"

Colleague::Colleague(NurseryMediator* med): mediator(med){}

Colleague::~Colleague(){}

void Colleague::setMediator(NurseryMediator* med){
    mediator = med;
}

NurseryMediator* Colleague::getMediator()const{
    return mediator;
}