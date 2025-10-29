#include "../include/SalesFloor.h"
#include "../include/Plant.h"
#include "../include/Customer.h"
#include <iostream>
#include <algorithm>
#include <sstream>

SalesFloor::SalesFloor(NurseryMediator* med, int numRows, int numCols): Colleague(med), rows(numRows), cols(numCols), currentNumberOfPlants(0){
    
    capacity = rows * cols;
    
    displayGrid.resize(rows);

    for(int i = 0; i < rows; i++){
        displayGrid[i].resize(cols, nullptr);
    }
    
    std::cout << "Sales floor created with " << rows << "x" << cols << " grid\n";
}

SalesFloor::~SalesFloor() {
    // Delete all plants still on display
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (displayGrid[i][j] != nullptr) {
                delete displayGrid[i][j];
                displayGrid[i][j] = nullptr;
            }
        }
        displayGrid[i].clear();
    }
    displayGrid.clear();
    currentCustomers.clear();
}

bool SalesFloor::addPlantToDisplay(Plant* plant, int row, int col){
    if(plant == nullptr){
        std::cout << "Cannot add null plant to display\n";
        return false;
    }
    
    if(row < 0 || row >= rows || col < 0 || col >= cols){
        std::cout << "Invalid position (" << row << "," << col << ")\n";
        return false;
    }
    
    if(displayGrid[row][col] != nullptr){
        std::cout << "Display position (" << row << "," << col << ") is occupied\n";
        return false;
    }
    
    if(isFull()){
        std::cout << "Sales floor is at full capacity\n";
        return false;
    }
    
    displayGrid[row][col] = plant;
    currentNumberOfPlants++;
    
    std::cout << "Plant " << plant->getID() << " added to sales floor display at (" << row << "," << col << ")\n";
    
    if(mediator != nullptr){
        mediator->notify(this);
    }
    
    return true;
}

void SalesFloor::removePlantFromDisplay(Plant* plant){
    if(plant == nullptr){
        return;
    }
    
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            if(displayGrid[i][j] == plant){
                displayGrid[i][j] = nullptr;
                currentNumberOfPlants--;
                
                std::cout << "Plant " << plant->getID() << " removed from sales floor\n";
                
                if(mediator != nullptr){
                    mediator->notify(this);
                }
                
                return;
            }
        }
    }
}

Plant* SalesFloor::removePlantAt(int row, int col){
    if(row < 0 || row >= rows || col < 0 || col >= cols){
        return nullptr;
    }
    
    Plant* plant = displayGrid[row][col];
    
    if(plant != nullptr){
        displayGrid[row][col] = nullptr;
        currentNumberOfPlants--;
        
        std::cout << "Plant removed from sales floor at (" << row << "," << col << ")\n";
        
        if(mediator != nullptr){
            mediator->notify(this);
        }
    }
    
    return plant;
}

Plant* SalesFloor::getPlantAt(int row, int col)const{
    if(row < 0 || row >= rows || col < 0 || col >= cols){
        return nullptr;
    }

    return displayGrid[row][col];
}

void SalesFloor::addCustomer(Customer* customer){
    if(customer != nullptr){
        currentCustomers.push_back(customer);

        std::cout << "Customer " << customer->getId() << " entered the sales floor\n";
        
        if(mediator != nullptr){
            mediator->notify(this);
        }
    }
}

void SalesFloor::removeCustomer(Customer* customer){
    auto it = std::find(currentCustomers.begin(), currentCustomers.end(), customer);

    if(it != currentCustomers.end()){
        currentCustomers.erase(it);
        std::cout << "Customer " << customer->getId() << " left the sales floor\n";
        
        if(mediator != nullptr){
            mediator->notify(this);
        }
    }
}

std::vector<Plant*> SalesFloor::getDisplayPlants()const{
    std::vector<Plant*> allPlants;
    
    for(int i = 0; i < rows; i++){ // flatted grid into a vector
        for(int j = 0; j < cols; j++){

            if(displayGrid[i][j] != nullptr){
                allPlants.push_back(displayGrid[i][j]);
            }
        }
    }
    
    return allPlants;
}

std::vector<Customer*> SalesFloor::getCurrentCustomers()const{
    return currentCustomers;
}

std::vector<Plant*> SalesFloor::getPlantsInRow(int row)const{
    std::vector<Plant*> rowPlants;
    
    if(row < 0 || row >= rows){
        return rowPlants;
    }
    
    for(int j = 0; j < cols; j++){

        if(displayGrid[row][j] != nullptr){
            rowPlants.push_back(displayGrid[row][j]);
        }
    }
    
    return rowPlants;
}

std::vector<Plant*> SalesFloor::getPlantsInColumn(int col)const{
    std::vector<Plant*> colPlants;
    
    if(col < 0 || col >= cols){
        return colPlants;
    }
    
    for(int i = 0; i < rows; i++){

        if(displayGrid[i][col] != nullptr){
            colPlants.push_back(displayGrid[i][col]);
        }
    }
    
    return colPlants;
}

int SalesFloor::getNumberOfPlants()const{
    return currentNumberOfPlants;
}

int SalesFloor::getCapacity()const{
    return capacity;
}

bool SalesFloor::isFull()const{
    return currentNumberOfPlants >= capacity;
}

int SalesFloor::getRows()const{
    return rows;
}

int SalesFloor::getColumns()const{
    return cols;
}

bool SalesFloor::isPositionEmpty(int row, int col)const{
    if(row < 0 || row >= rows || col < 0 || col >= cols){
        return false;
    }
    
    return displayGrid[row][col] == nullptr;
}

std::string SalesFloor::toString() const {
    std::ostringstream output;
    output << "=== SALES FLOOR STATUS ===\n";
    output << "Plants on Display: " << currentNumberOfPlants << "/" << capacity << "\n";
    output << "Current Customers: " << currentCustomers.size() << "\n";
    output << "Grid Size: " << rows << "x" << cols << "\n\n";
    
    if (currentNumberOfPlants == 0) {
        output << "No plants on display.\n";
    } else {
        output << "Plants for Sale:\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Plant* plant = displayGrid[i][j];
                if (plant != nullptr) {
                    output << "  Position (" << i << "," << j << "): "
                           << plant->getName() << " (ID: " << plant->getID() 
                           << ") - R" << plant->getPrice() << "\n";
                }
            }
        }
    }
    
    if (!currentCustomers.empty()) {
        output << "\nCustomers Shopping:\n";
        for (Customer* customer : currentCustomers) {
            if (customer != nullptr) {
                output << "  - " << customer->getName() 
                       << " (ID: " << customer->getId() << ")\n";
            }
        }
    }
    
    return output.str();
}