#include "../include/NurseryCoordinator.h"
#include "../include/SalesFloor.h"
#include "../include/Greenhouse.h"
#include "../include/Plant.h"
#include "../include/Person.h"
#include <iostream>

NurseryCoordinator::NurseryCoordinator(): NurseryMediator(), salesFloorRef(nullptr), greenhouseRef(nullptr){}

NurseryCoordinator::~NurseryCoordinator(){}

void NurseryCoordinator::setSalesFloor(SalesFloor* sf){
    salesFloorRef = sf;
    std::cout << "NurseryCoordinator: Sales floor reference set\n";
}

void NurseryCoordinator::setGreenhouse(Greenhouse* gh){
    greenhouseRef = gh;
    std::cout << "NurseryCoordinator: Greenhouse reference set\n";
}

void NurseryCoordinator::checkPlantRelocation(){
    if(greenhouseRef == nullptr || salesFloorRef == nullptr){
        std::cout << "NurseryCoordinator: Cannot check relocation, references missing\n";

        return;
    }

    std::cout << "NurseryCoordinator: Checking for plants ready to move to sales floor\n";
    
    std::vector<Plant*> allPlants = greenhouseRef->getAllPlants();
    
    for(Plant* plant: allPlants){
        if(plant != nullptr && plant->isReadyForSale()){
            std::cout << "NurseryCoordinator: Plant '" << plant->getName() << "' (ID: " << plant->getID() << ") is ready for sale\n";
            
            // find an empty spot on the sales floor to put the plant
            bool placed = false;

            for(int i = 0; i < salesFloorRef->getRows() && !placed; i++){
                for(int j = 0; j < salesFloorRef->getColumns() && !placed; j++){

                    if(salesFloorRef->isPositionEmpty(i, j)){
                        greenhouseRef->removePlant(plant);
                        salesFloorRef->addPlantToDisplay(plant, i, j);

                        placed = true;

                        std::cout << "NurseryCoordinator: Moved plant '" << plant->getName() << "' (ID: " << plant->getID() << ") to the sales floor at position (" << i << "," << j << ")\n";
                    }
                }
            }
            
            if(!placed){
                std::cout << "NurseryCoordinator: Sales floor is full, cannot move plant\n";
            }
        }
    }
}

bool NurseryCoordinator::coordinatePlantTransfer(std::string plantName){
    if(greenhouseRef == nullptr || salesFloorRef == nullptr){
        std::cout << "NurseryCoordinator: Cannot transfer, references missing\n";
        return false;
    }

    std::cout << "NurseryCoordinator: Attempting to transfer '" << plantName << "'\n";
    
    Plant* plant = greenhouseRef->findPlant(plantName);

    if(plant == nullptr){
        std::cout << "NurseryCoordinator: Plant not found in greenhouse\n";
        return false;
    }
    
    if(!plant->isReadyForSale()){
        std::cout << "NurseryCoordinator: Plant is not ready for sale yet\n";
        return false;
    }
    
    // finding an empty spot on the sales floor
    for(int i = 0; i < salesFloorRef->getRows(); i++){
        for(int j = 0; j < salesFloorRef->getColumns(); j++){

            if(salesFloorRef->isPositionEmpty(i, j)){
                greenhouseRef->removePlant(plant);
                salesFloorRef->addPlantToDisplay(plant, i, j);
                
                std::cout << "NurseryCoordinator: Successfully transferred the plant to sales floor\n";
                return true;
            }
        }
    }
    
    std::cout << "NurseryCoordinator: Sales floor is full\n";
    return false;
}

std::string NurseryCoordinator::assignStaffToCustomer(std::string customerId){
    std::cout << "NurseryCoordinator: Assigning staff to customer " << customerId << "\n";
    
    // this will be enhanced when staff chain of responsibility is implemented
    Person* staff = getAvailableStaff();

    if(staff != nullptr){
        std::cout << "NurseryCoordinator: Assigned staff member " << staff->getId() << " to customer " << customerId << "\n";

        return staff->getId();
    }
    
    std::cout << "NurseryCoordinator: No staff available at the moment\n";
    return "None"; // no staff member assigned at the moment
}

Person* NurseryCoordinator::getAvailableStaff(){
    for(Colleague* colleague: colleagues){
        Person* person = dynamic_cast<Person*>(colleague);

        if(person != nullptr){
            if(person->getId().find("Cust") == std::string::npos && // make sure of these names/ ids
               person->getId().find("Corp") == std::string::npos && 
               person->getId().find("Reg") == std::string::npos && 
               person->getId().find("Walk") == std::string::npos){

                return person;
            }
        }
    }

    return nullptr;
}

bool NurseryCoordinator::coordinatePurchaseWorkflow(std::string customerId, std::string plantName){
    std::cout << "NurseryCoordinator: Coordinating purchase workflow for customer " << customerId << " requesting '" << plantName << "'\n";
    
    if(salesFloorRef != nullptr){
        std::vector<Plant*> displayPlants = salesFloorRef->getDisplayPlants();

        for(Plant* plant: displayPlants){
            
            if(plant != nullptr && plant->getName() == plantName){
                std::cout << "NurseryCoordinator: Plant found on sales floor, processing purchase\n";
                processPurchase();

                return true;
            }
        }
    }
    
    if(greenhouseRef != nullptr && greenhouseRef->hasPlant(plantName)){
        std::cout << "NurseryCoordinator: Plant found in greenhouse\n";
        
        Plant* plant = greenhouseRef->findPlant(plantName);

        if(plant != nullptr && plant->isReadyForSale()){
            std::cout << "NurseryCoordinator: Plant is ready, transferring to sales floor\n";

            if(coordinatePlantTransfer(plantName)){
                processPurchase();
                return true;
            }
        } 
        else{
            std::cout << "NurseryCoordinator: Plant is still growing, informing customer\n";

            return false;
        }
    }
    
    std::cout << "NurseryCoordinator: " << plantUnavailable() << "\n";

    return false;
}