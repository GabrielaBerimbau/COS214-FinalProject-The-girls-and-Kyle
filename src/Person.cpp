#include "../include/Person.h"

Person::Person(NurseryMediator* med, std::string personName, std::string personId): Colleague(med), name(personName), id(personId) {}

Person::~Person(){}

std::string Person::getName()const{
    return name;
}

std::string Person::getId()const{
    return id;
}

void Person::setName(std::string personName){
    name = personName;
}

void Person::setId(std::string personId){
    id = personId;
}