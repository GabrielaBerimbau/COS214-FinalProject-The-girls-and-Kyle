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
                std::cout << "Mediator: '" << plantName << "' plant found in the greenhouse\n";
                return plantType;
            }
        }

        SalesFloor* sf = dynamic_cast<SalesFloor*>(colleague);

        if(sf != nullptr){
            std::vector<Plant*> displayPlants = sf->getDisplayPlants();

            for(Plant* plant: displayPlants){
                if(plant != nullptr && (plant->getName() == plantName)){
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
    
    std::cout << "Mediator: Giving plant '" << plant->getName() << "' (ID: " << plant->getID() << ") to customer\n";
    
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

bool NurseryMediator::transferPlantToCustomer(std::string plantName, Customer* customer){
    if(customer == nullptr){
        std::cout << "Mediator: Cannot transfer to a null customer\n";

        return false;
    }
    
    std::cout << "Mediator: Transferring '" << plantName << "' to customer " << customer->getName() << "\n";
    
    Plant* plant = nullptr;
    
    // first try to find on salesfloor and then transfer it
    for(Colleague* colleague: colleagues) {
        SalesFloor* sf = dynamic_cast<SalesFloor*>(colleague);

        if(sf != nullptr){
            std::vector<Plant*> displayPlants = sf->getDisplayPlants();

            for(Plant* p: displayPlants){
                if(p != nullptr && p->getName() == plantName){
                    plant = p;

                    sf->removePlantFromDisplay(plant);
                    std::cout << "Mediator: Removed the plant from sales floor\n";
    
                    break;
                }
            }

            if(plant != nullptr){
                break;
            } 
        }
    }
    
    // try the greenhouse if it isn't on the salesfloor
    if(plant == nullptr){
        for(Colleague* colleague : colleagues){
            Greenhouse* gh = dynamic_cast<Greenhouse*>(colleague);

            if(gh != nullptr){
                plant = gh->findPlant(plantName);

                if(plant != nullptr){

                    if(!plant->isReadyForSale()){
                        std::cout << "Mediator: Plant is not ready for sale yet\n";
                        return false;
                    }

                    gh->removePlant(plant);
                    std::cout << "Mediator: Removed plant from the greenhouse\n";

                    break;
                }
            }
        }
    }
    
    // add the plant to a customers cart if it was found
    if(plant != nullptr){
        customer->addToCart(plant);
        std::cout << "Mediator: Successfully transferred plant to customer\n";
        
        return true;
    }
    
    std::cout << "Mediator: Plant '" << plantName << "' not found\n";
    return false;
}

bool NurseryMediator::returnPlantToDisplay(Plant* plant){
    if(plant == nullptr){
        std::cout << "Mediator: Cannot return a null plant\n";
        return false;
    }
    
    std::cout << "Mediator: Returning plant '" << plant->getName() << "' to sales floor\n";
    
    // add plant back to the salesfloor
    for(Colleague* colleague: colleagues){
        SalesFloor* sf = dynamic_cast<SalesFloor*>(colleague);

        if(sf != nullptr){
            for(int i = 0; i < sf->getRows(); i++){
                for(int j = 0; j < sf->getColumns(); j++){

                    if(sf->isPositionEmpty(i, j)){
                        bool successSF = sf->addPlantToDisplay(plant, i, j);

                        if(successSF){
                            std::cout << "Mediator: Plant returned to sales floor at (" << i << "," << j << ")\n";

                            return true;
                        }
                    }
                }
            }
            std::cout << "Mediator: Sales floor is full, cannot return plant\n";
            return false;
        }
    }
    
    std::cout << "Mediator: Sales floor not found\n";
    return false;
}