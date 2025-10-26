#include "../include/StaffMembers.h"
#include <iostream>

StaffMembers::StaffMembers(): nextHandler(nullptr){}

StaffMembers::~StaffMembers(){}

void StaffMembers::setNext(StaffMembers* next){
    nextHandler = next;
    std::cout << "Chain: Handler linked to next handler\n";
}

void StaffMembers::handleRequest(){
    std::cout << "StaffMembers: Base handleRequest called\n";
}