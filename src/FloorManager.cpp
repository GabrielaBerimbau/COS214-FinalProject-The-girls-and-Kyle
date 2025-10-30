/**
 * @file FloorManager.cpp
 * @brief Short implementation notes for FloorManager.
 *
 * Implementation details for the `FloorManager` request handling. See the
 * header (`FloorManager.h`) for full responsibilities and usage examples.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-30
 */

#include "../include/FloorManager.h"
#include <iostream>
#include <cstdlib>

FloorManager::FloorManager(NurseryMediator* med, std::string staffName, std::string staffId): StaffMembers(med, staffName, staffId){}

FloorManager::~FloorManager(){}

void FloorManager::handleRequest(Request* request){
    if(request == nullptr){
        std::cout << "FloorManager: Received null request\n";
        return;
    }
    
    std::cout << "FloorManager " << getId() << ": Received request - '" << request->getMessage() << "'\n";
    
    // medium requests - bulk orders, special plant arrangements
    if(request->getLevel() == RequestLevel::MEDIUM){
        std::cout << "FloorManager " << getId() << ": Handling moderate complexity request\n";
        
        std::string message = request->getMessage();
        
        if(message.find("bulk") != std::string::npos || // double check these
           message.find("wedding") != std::string::npos ||
           message.find("special") != std::string::npos){
            
            std::cout << "FloorManager " << getId() << ": Processing bulk/ special order\n";
        }
        
        request->markHandled();
    } 
    else if(request->getLevel() == RequestLevel::LOW){
        // shouldn't get here, but handle just in case
        std::cout << "FloorManager " << getId() << ": Handling simple request\n";
        request->markHandled();
    } 
    
    else{
        if(nextHandler != nullptr){
            std::cout << "FloorManager " << getId() << ": Escalating to Nursery Owner\n";
            nextHandler->handleRequest(request);
        } 

        else{
            std::cout << "FloorManager " << getId() << ": No higher authority available\n";
        }
    }
}

void FloorManager::handleRequest(){
    std::cout << "FloorManager " << getId() << ": Handling general request\n";
}
