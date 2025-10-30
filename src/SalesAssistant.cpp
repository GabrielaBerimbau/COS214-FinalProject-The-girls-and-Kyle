/**
 * @file SalesAssistant.cpp
 * @brief Short implementation notes for SalesAssistant.
 *
 * See `SalesAssistant.h` for the class responsibilities and usage notes.
 * This file contains the concrete request-handling logic used by the
 * sales assistant role. Keep only short descriptions here; detailed
 * behavioural documentation belongs in the header.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-30
 */

#include "../include/SalesAssistant.h"
#include "../include/Customer.h"
#include <iostream>

SalesAssistant::SalesAssistant(NurseryMediator* med, std::string staffName, std::string staffId)
    : StaffMembers(med, staffName, staffId), scheduler(new CareScheduler()){}

SalesAssistant::~SalesAssistant(){
    delete scheduler;
}

void SalesAssistant::handleRequest(Request* request){
    if(request == nullptr){
        std::cout << "[SalesAssistant] Received null request\n";
        return;
    }
    
    std::cout << "[SalesAssistant " << getId() << "] Received request - '" 
              << request->getMessage() << "'\n";
    
    // Handle LOW level requests - simple plant requests
    if(request->getLevel() == RequestLevel::LOW){
        std::cout << "[SalesAssistant " << getId() << "] Handling simple request\n";
        
        std::string plantName = request->extractPlantName();
        Customer* customer = request->getCustomer();
        
        if(!plantName.empty() && customer != nullptr && mediator != nullptr){
            std::cout << "[SalesAssistant " << getId() << "] Customer wants a '" 
                      << plantName << "'\n";
            
            // Use mediator to add plant to customer's cart
            bool success = mediator->staffAddPlantToCustomerCart(plantName, customer);
            
            if(success) {
                customer->receiveResponse("Here's your " + plantName + "! It's been added to your cart.");
                request->markHandled();
            } else {
                customer->receiveResponse("Sorry, we don't have that plant available right now.");
                request->markHandled();
            }
        } else {
            std::cout << "[SalesAssistant " << getId() << "] Processing general inquiry\n";
            if(customer != nullptr) {
                customer->receiveResponse("I'm here to help! What can I do for you?");
            }
            request->markHandled();
        }
    } else {
        // Escalate to next handler
        if(nextHandler != nullptr){
            std::cout << "[SalesAssistant " << getId() << "] Escalating request to next handler\n";
            nextHandler->handleRequest(request);
        } else {
            std::cout << "[SalesAssistant " << getId() << "] No higher authority available\n";
        }
    }
}

void SalesAssistant::handleRequest(){
    std::cout << "[SalesAssistant " << getId() << "] Handling general request\n";
}

void SalesAssistant::customerRequestsPlant(std::string plantName){
    std::cout << "[SalesAssistant " << getId() << "] Customer requested '" << plantName << "'\n";
    
    Plant* plant = findRequestedPlant(plantName);
    
    if(plant != nullptr){
        std::cout << "[SalesAssistant " << getId() << "] Found plant '" << plantName << "'\n";
    } else {
        std::cout << "[SalesAssistant " << getId() << "] Plant '" << plantName << "' not found\n";
    }
}

Plant* SalesAssistant::findRequestedPlant(std::string plantName){
    std::cout << "[SalesAssistant " << getId() << "] Searching for '" << plantName << "'\n";

    if(mediator != nullptr){
        return mediator->requestPlantFromStaff(plantName);
    }

    return nullptr;
}

void SalesAssistant::runCareScheduler(){
    if(scheduler != nullptr && !scheduler->empty()){
        std::cout << "[SalesAssistant " << getId() << "] Running care scheduler\n";
        scheduler->runAll();
    }
}

