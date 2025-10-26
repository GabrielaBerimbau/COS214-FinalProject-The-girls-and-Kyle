#include "include/Colleague.h"

Colleague::Colleague(NurseryMediator* m) : mediator(m) {
}

Colleague::~Colleague() {
}

void Colleague::setMediator(NurseryMediator* m) {
    mediator = m;
}

NurseryMediator* Colleague::getMediator() const {
    return mediator;
}