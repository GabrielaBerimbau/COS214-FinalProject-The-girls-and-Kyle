#include "../include/SalesAssistant.h"
#include <iostream>
#include <cstdlib>

SalesAssistant::SalesAssistant(NurseryMediator* med, std::string staffName, std::string staffId): StaffMembers(med, staffName, staffId), scheduler(new CareScheduler()){}

SalesAssistant::~SalesAssistant(){
    delete scheduler;
}

void SalesAssistant::handleRequest(Request* request){
    if(request == nullptr){
        std::cout << "SalesAssistant: Received null request\n";
        return;
    }
    
    std::cout << "SalesAssistant " << getId() << ": Received request - '" << request->getMessage() << "'\n";
    
    // low level requests - simple plant requests
    if(request->getLevel() == RequestLevel::LOW){
        std::cout << "SalesAssistant " << getId() << ": Handling simple request\n";
        
        std::string message = request->getMessage();
        
        // if(message.find("plant") != std::string::npos || 
        //    message.find("rose") != std::string::npos || 
        //    message.find("daisy") != std::string::npos ||
        //    message.find("cactus") != std::string::npos){
            
        //     std::cout << "SalesAssistant " << id << ": Processing plant request\n";
        // }

        // waiting for request.cpp to be complete
        
        request->markHandled();
    } 
    else{
        if(nextHandler != nullptr){
            std::cout << "SalesAssistant " << getId() << ": Escalating request to next handler\n";
            nextHandler->handleRequest(request);
        } 
        
        else{
            std::cout << "SalesAssistant " << getId() << ": No higher authority available\n";
        }
    }
}

void SalesAssistant::handleRequest(){
    std::cout << "SalesAssistant " << getId() << ": Handling general request\n";
}

void SalesAssistant::customerRequestsPlant(std::string plantName){
    std::cout << "SalesAssistant " << getId() << ": Customer requested '" << plantName << "'\n";
    
    Plant* plant = findRequestedPlant(plantName);
    
    if(plant != nullptr){
        std::cout << "SalesAssistant " << getId() << ": Found plant '" << plantName << "'\n";
    } 

    else{
        std::cout << "SalesAssistant " << getId() << ": Plant '" << plantName << "' not found\n";
    }
}

Plant* SalesAssistant::findRequestedPlant(std::string plantName){
    std::cout << "SalesAssistant " << getId() << ": Searching for '" << plantName << "'\n";

    if(mediator != nullptr){
        return mediator->requestPlantFromStaff(plantName);
    }

    return nullptr;
}

void SalesAssistant::runCareScheduler(){
    if(scheduler != nullptr && !scheduler->empty()){
        std::cout << "SalesAssistant " << getId() << ": Running care scheduler\n";
        scheduler->runAll();
    }
}