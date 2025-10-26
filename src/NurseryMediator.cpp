#include "../include/NurseryMediator.h"
#include "../include/Colleague.h"
#include "../include/Plant.h"
#include "../include/Greenhouse.h"
#include "../include/SalesFloor.h"
#include "../include/Customer.h"

#include <iostream>
#include <algorithm>

NurseryMediator::NurseryMediator(){}

NurseryMediator::~NurseryMediator(){
    colleagues.clear(); // the delete will be manages somewhere else
}

void NurseryMediator::notify(Colleague* colleague){
    if(colleague == nullptr){
        return;
    }

    std::cout << "Mediator received notification from the colleague\n"; 
    // should specific colleague type handling be here? e.g. customer etc
}

void NurseryMediator::processPurchase(){
    std::cout << "Mediator processing purchase\n"; // double check
}

Plant* NurseryMediator::requestPlantFromStaff(std::string plantName){
    std::cout << "Mediator: requesting '" << plantName << "' from staff\n";

    for(Colleague* colleague: colleagues){
        Greenhouse* gh = dynamic_cast<Greenhouse*>(colleague);

        if(gh != nullptr){
            Plant* plantType = gh->findPlant(plantName);

            if(plantType != nullptr){
                std::cout << "Mediator: '" << plantName << "' plant found in the greenhousen\n";
                return plantType;
            }
        }

        SalesFloor* sf = dynamic_cast<SalesFloor*>(colleague);

        if(sf != nullptr){
            std::vector<Plant*> displayPlants = sf->getDisplayPlants();

            for(Plant* plant: displayPlants){
                if(plant != nullptr && (plant->getID() == plantName)){
                    std::cout << "Mediator: Plant was found on sales floor" << std::endl;
                    return plant;
                }
            }
        }
    }

    std::cout << "Mediator: Plant was not found\n";
    return nullptr;
}

bool NurseryMediator::staffChecksGreenHouse(std::string plantName){
    std::cout << "Mediator: Checking the greenhouse for '" << plantName << "'\n";
    
    for (Colleague* colleague: colleagues){
        Greenhouse* gh = dynamic_cast<Greenhouse*>(colleague);

        if (gh != nullptr) {
            bool hasPlant = gh->hasPlant(plantName);
            std::cout << "Mediator: Greenhouse " << (hasPlant ? "has " : "doesn't have ") << "plant\n";

            return hasPlant;
        }
    }
    
    std::cout << "Mediator: greenhouse not found" << std::endl;
    return false;
}

void NurseryMediator::giveCustomerPlant(Plant* plant){
    if(plant == nullptr){
        std::cout << "Mediator: Cannot give null plant to customer\n";
        return;
    }
    
    std::cout << "Mediator: Giving plant " << plant->getID() << " to customer\n";
    
    for (Colleague* colleague: colleagues) {
        Customer* customer = dynamic_cast<Customer*>(colleague);

        if(customer != nullptr){
            customer->giveCustomerPlant(plant);
            break;
        }
    }
}

std::string NurseryMediator::plantUnavailable() {
    return "The requested plant is unfortunately unavailable. Would you like to see our other options?";
}

void NurseryMediator::registerColleague(Colleague* colleague) {
    if (colleague != nullptr){
        auto it = std::find(colleagues.begin(), colleagues.end(), colleague);

        if(it == colleagues.end()) {
            colleagues.push_back(colleague);
            std::cout << "Mediator: Colleague registered\n";
        }
    }
}

void NurseryMediator::removeColleague(Colleague* colleague) {
    auto it = std::find(colleagues.begin(), colleagues.end(), colleague);

    if(it != colleagues.end()){
        colleagues.erase(it);
        std::cout << "Mediator: Colleague removed\n";
    }
}