
#include "../include/Colleague.h"

Colleague::Colleague(NurseryMediator* med): mediator(med){}

Colleague::~Colleague(){}

void Colleague::setMediator(NurseryMediator* med){
    mediator = med;
}

NurseryMediator* Colleague::getMediator()const{
    return mediator;
}