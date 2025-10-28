#include "../include/Colleague.h" // since the file is in a diff directory (and one back)

Colleague::Colleague(NurseryMediator* med): mediator(med){}

Colleague::~Colleague(){}

void Colleague::setMediator(NurseryMediator* med){
    mediator = med;
}

NurseryMediator* Colleague::getMediator()const{
    return mediator;
}