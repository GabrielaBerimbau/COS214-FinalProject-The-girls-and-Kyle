/**
 * @file StaffMembers.cpp
 * @brief Implementation for staff chain base class.
 *
 * See `StaffMembers.h` for the full design and usage details of the
 * Chain of Responsibility used by staff. This source file contains
 * the simple, implementation-level functions shared by all handlers.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-30
 */

#include "../include/StaffMembers.h"
#include <iostream>

StaffMembers::StaffMembers(NurseryMediator* med, std::string staffName, std::string staffId): Person(med, staffName, staffId), nextHandler(nullptr){}

StaffMembers::~StaffMembers(){}

void StaffMembers::setNext(StaffMembers* next){
    nextHandler = next;
    std::cout << "Chain: Handler linked to next handler\n";
}

void StaffMembers::handleRequest(){
    std::cout << "StaffMembers: Base handleRequest called\n";
}