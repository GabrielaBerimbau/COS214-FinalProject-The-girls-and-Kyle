#include "../include/FloorManager.h"
#include <iostream>
#include <cstdlib>

FloorManager::FloorManager(): StaffMembers(), name("Floor Manager"), id("FM" + std::to_string(rand() % 1000)){}

FloorManager::FloorManager(std::string staffName, std::string staffId): StaffMembers(), name(staffName), id(staffId){}

FloorManager::~FloorManager(){}

void FloorManager::handleRequest(Request* request){
    if(request == nullptr){
        std::cout << "FloorManager: Received null request\n";
        return;
    }
    
    std::cout << "FloorManager " << id << ": Received request - '" << request->getMessage() << "'\n";
    
    // medium requests - bulk orders, special plant arracngements
    if(request->getLevel() == RequestLevel::MEDIUM){
        std::cout << "FloorManager " << id << ": Handling moderate complexity request\n";
        
        std::string message = request->getMessage();
        
        if(message.find("bulk") != std::string::npos || // double check these
           message.find("wedding") != std::string::npos ||
           message.find("special") != std::string::npos){
            
            std::cout << "FloorManager " << id << ": Processing bulk/ special order\n";
        }
        
        request->markHandled();
    } 
    else if(request->getLevel() == RequestLevel::LOW){
        // shouldn't get here, but handle just in case
        std::cout << "FloorManager " << id << ": Handling simple request\n";
        request->markHandled();
    } 
    
    else{
        if(nextHandler != nullptr){
            std::cout << "FloorManager " << id << ": Escalating to Nursery Owner\n";
            nextHandler->handleRequest(request);
        } 

        else{
            std::cout << "FloorManager " << id << ": No higher authority available\n";
        }
    }
}

void FloorManager::handleRequest(){
    std::cout << "FloorManager " << id << ": Handling general request\n";
}

std::string FloorManager::getName()const{
    return name;
}

std::string FloorManager::getId()const{
    return id;
}