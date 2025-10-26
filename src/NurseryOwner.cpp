#include "../include/NurseryOwner.h"
#include <iostream>
#include <cstdlib>

NurseryOwner::NurseryOwner(NurseryMediator* med, std::string staffName, std::string staffId): StaffMembers(med, staffName, staffId){}

NurseryOwner::~NurseryOwner(){}

void NurseryOwner::handleRequest(Request* request){
    if(request == nullptr){
        std::cout << "NurseryOwner: Received null request\n";
        return;
    }
    
    std::cout << "NurseryOwner " << id << ": Received request - '" << request->getMessage() << "'\n";

    std::cout << "NurseryOwner " << id << ": Making decision on request\n";
    
    std::string message = request->getMessage();
    
    // handle complaints, refunds
    if(message.find("complaint") != std::string::npos || message.find("refund") != std::string::npos){
        
        std::cout << "NurseryOwner " << id << ": Addressing customer complaint/ refund\n";
    } 
    else if(message.find("lawsuit") != std::string::npos ||  message.find("urgent") != std::string::npos){
        
        std::cout << "NurseryOwner " << id << ": Handling urgent/ legal matter\n";
    }
    
    else{
        std::cout << "NurseryOwner " << id << ": Making final decision on complex request\n";
    }
    
    request->markHandled();
    std::cout << "NurseryOwner " << id << ": Request resolved\n";
}

void NurseryOwner::handleRequest(){
    std::cout << "NurseryOwner " << id << ": Handling general request\n";
}

std::string NurseryOwner::getName()const{
    return name;
}

std::string NurseryOwner::getId()const{
    return id;
}