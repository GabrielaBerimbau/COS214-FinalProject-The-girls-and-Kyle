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
    colleagues.clear();
}

void NurseryMediator::notify(Colleague* colleague){
    if(colleague == nullptr){
        return;
    }

    std::cout << "[Mediator] Received notification from colleague\n"; 
}

void NurseryMediator::processPurchase(){
    std::cout << "[Mediator] Processing purchase\n";
}

Plant* NurseryMediator::requestPlantFromStaff(std::string plantName){
    std::cout << "[Mediator] Requesting '" << plantName << "' from staff\n";

    for(Colleague* colleague: colleagues){
        Greenhouse* gh = dynamic_cast<Greenhouse*>(colleague);

        if(gh != nullptr){
            Plant* plantType = gh->findPlant(plantName);

            if(plantType != nullptr){
                std::cout << "[Mediator] '" << plantName << "' found in greenhouse\n";
                return plantType;
            }
        }

        SalesFloor* sf = dynamic_cast<SalesFloor*>(colleague);

        if(sf != nullptr){
            std::vector<Plant*> displayPlants = sf->getDisplayPlants();

            for(Plant* plant: displayPlants){
                if(plant != nullptr && (plant->getName() == plantName)){
                    std::cout << "[Mediator] Plant found on sales floor\n";
                    return plant;
                }
            }
        }
    }

    std::cout << "[Mediator] Plant not found\n";
    return nullptr;
}

bool NurseryMediator::staffChecksGreenHouse(std::string plantName){
    std::cout << "[Mediator] Checking greenhouse for '" << plantName << "'\n";
    
    for (Colleague* colleague: colleagues){
        Greenhouse* gh = dynamic_cast<Greenhouse*>(colleague);

        if (gh != nullptr) {
            bool hasPlant = gh->hasPlant(plantName);
            std::cout << "[Mediator] Greenhouse " << (hasPlant ? "has " : "doesn't have ") << "plant\n";

            return hasPlant;
        }
    }
    
    std::cout << "[Mediator] Greenhouse not found\n";
    return false;
}

std::string NurseryMediator::plantUnavailable() {
    return "The requested plant is unfortunately unavailable. Would you like to see our other options?";
}

void NurseryMediator::registerColleague(Colleague* colleague) {
    if (colleague != nullptr){
        auto it = std::find(colleagues.begin(), colleagues.end(), colleague);

        if(it == colleagues.end()) {
            colleagues.push_back(colleague);
            std::cout << "[Mediator] Colleague registered\n";
        }
    }
}

void NurseryMediator::removeColleague(Colleague* colleague) {
    auto it = std::find(colleagues.begin(), colleagues.end(), colleague);

    if(it != colleagues.end()){
        colleagues.erase(it);
        std::cout << "[Mediator] Colleague removed\n";
    }
}

bool NurseryMediator::transferPlantToCustomer(std::string plantName, Customer* customer){
    if(customer == nullptr){
        std::cout << "[Mediator] Cannot transfer to null customer\n";
        return false;
    }
    
    std::cout << "[Mediator] Transferring '" << plantName << "' to " << customer->getName() << "\n";
    
    Plant* plant = nullptr;
    
    // First try sales floor
    for(Colleague* colleague: colleagues) {
        SalesFloor* sf = dynamic_cast<SalesFloor*>(colleague);

        if(sf != nullptr){
            std::vector<Plant*> displayPlants = sf->getDisplayPlants();

            for(Plant* p: displayPlants){
                if(p != nullptr && p->getName() == plantName){
                    plant = p;
                    sf->removePlantFromDisplay(plant);
                    std::cout << "[Mediator] Removed plant from sales floor\n";
                    break;
                }
            }

            if(plant != nullptr){
                break;
            } 
        }
    }
    
    // Try greenhouse if not on sales floor
    if(plant == nullptr){
        for(Colleague* colleague : colleagues){
            Greenhouse* gh = dynamic_cast<Greenhouse*>(colleague);

            if(gh != nullptr){
                plant = gh->findPlant(plantName);

                if(plant != nullptr){
                    if(!plant->isReadyForSale()){
                        std::cout << "[Mediator] Plant not ready for sale yet\n";
                        return false;
                    }

                    gh->removePlant(plant);
                    std::cout << "[Mediator] Removed plant from greenhouse\n";
                    break;
                }
            }
        }
    }
    
    // Transfer to customer if found
    if(plant != nullptr){
        customer->addToCart(plant);
        std::cout << "[Mediator] Successfully transferred plant to customer\n";
        return true;
    }
    
    std::cout << "[Mediator] Plant '" << plantName << "' not found\n";
    return false;
}

bool NurseryMediator::transferPlantFromPosition(int row, int col, Customer* customer) {
    if(customer == nullptr){
        std::cout << "[Mediator] Cannot transfer to null customer\n";
        return false;
    }
    
    std::cout << "[Mediator] Transferring plant at (" << row << "," << col 
              << ") to " << customer->getName() << "\n";
    
    // Find sales floor
    for(Colleague* colleague: colleagues) {
        SalesFloor* sf = dynamic_cast<SalesFloor*>(colleague);
        
        if(sf != nullptr) {
            // Get plant at position
            Plant* plant = sf->getPlantAt(row, col);
            
            if(plant == nullptr) {
                std::cout << "[Mediator] No plant at position (" << row << "," << col << ")\n";
                return false;
            }
            
            // Remove from sales floor
            sf->removePlantFromDisplay(plant);
            
            // Add to customer cart (transfers ownership)
            customer->addToCart(plant);
            
            std::cout << "[Mediator] Successfully transferred plant from (" << row << "," << col 
                      << ") to customer's cart\n";
            return true;
        }
    }
    
    std::cout << "[Mediator] Sales floor not found\n";
    return false;
}

bool NurseryMediator::staffAddPlantToCustomerCart(std::string plantName, Customer* customer) {
    if(customer == nullptr){
        std::cout << "[Mediator] Cannot add to null customer's cart\n";
        return false;
    }
    
    std::cout << "[Mediator] Staff adding '" << plantName << "' to " 
              << customer->getName() << "'s cart\n";
    
    // Use existing transfer logic
    return transferPlantToCustomer(plantName, customer);
}

bool NurseryMediator::returnPlantToDisplay(Plant* plant){
    if(plant == nullptr){
        std::cout << "[Mediator] Cannot return null plant\n";
        return false;
    }
    
    std::cout << "[Mediator] Returning plant '" << plant->getName() << "' to sales floor\n";
    
    // Find sales floor and first empty position
    for(Colleague* colleague: colleagues){
        SalesFloor* sf = dynamic_cast<SalesFloor*>(colleague);

        if(sf != nullptr){
            // Find first empty position
            for(int i = 0; i < sf->getRows(); i++){
                for(int j = 0; j < sf->getColumns(); j++){
                    if(sf->isPositionEmpty(i, j)){
                        bool success = sf->addPlantToDisplay(plant, i, j);
                        if(success){
                            std::cout << "[Mediator] Plant returned to sales floor at (" 
                                      << i << "," << j << ")\n";
                            return true;
                        }
                    }
                }
            }
            
            std::cout << "[Mediator] Sales floor is full, cannot return plant\n";
            return false;
        }
    }
    
    std::cout << "[Mediator] Sales floor not found\n";
    return false;
}