
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
    
    std::cout << "NurseryOwner " << getId() << ": Received request - '" << request->getMessage() << "'\n";

    std::cout << "NurseryOwner " << getId() << ": Making decision on request\n";
    
    std::string message = request->getMessage();
    
    // handle complaints, refunds
    if(message.find("complaint") != std::string::npos || message.find("refund") != std::string::npos){
        
        std::cout << "NurseryOwner " << getId() << ": Addressing customer complaint/ refund\n";
    } 
    else if(message.find("lawsuit") != std::string::npos ||  message.find("urgent") != std::string::npos){
        
        std::cout << "NurseryOwner " << getId() << ": Handling urgent/ legal matter\n";
    }
    
    else{
        std::cout << "NurseryOwner " << getId() << ": Making final decision on complex request\n";
    }
    
    request->markHandled();
    std::cout << "NurseryOwner " << getId() << ": Request resolved\n";
}

void NurseryOwner::handleRequest(){
    std::cout << "NurseryOwner " << getId() << ": Handling general request\n";
}
