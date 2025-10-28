#include "../include/Greenhouse.h"
#include "../include/Plant.h"
#include <iostream>

Greenhouse::Greenhouse(NurseryMediator* med, int numRows, int numCols): Colleague(med), currentNumberOfPlants(0), rows(numRows), cols(numCols) {
    
    capacity = rows * cols;
    plantGrid.resize(rows);

    for(int i = 0; i < rows; i++){
        plantGrid[i].resize(cols, nullptr);
    }
    
    std::cout << "Greenhouse created with " << rows << "x" << cols << " grid\n";
}

Greenhouse::~Greenhouse(){
    for(int i = 0; i < rows; i++){ // plants will be deleted elsewhere
        plantGrid[i].clear();
    }

    plantGrid.clear();
}

bool Greenhouse::addPlant(Plant* plant, int row, int col){
    if (plant == nullptr){
        std::cout << "Cannot add a null plant\n";

        return false;
    }
    
    if(row < 0 || row >= rows || col < 0 || col >= cols){
        std::cout << "Invalid position (" << row << "," << col << ")\n";

        return false;
    }
    
    if(plantGrid[row][col] != nullptr){
        std::cout << "Position (" << row << "," << col << ") is occupied\n";

        return false;
    }
    
    if(isFull()){
        std::cout << "Greenhouse is at full capacity" << std::endl;

        return false;
    }
    
    plantGrid[row][col] = plant;
    currentNumberOfPlants++;
    
    std::cout << "Plant '" << plant->getName() << "' (ID: " << plant->getID() << ") was added to greenhouse at (" << row << "," << col << ")\n";

    if(mediator != nullptr){
        mediator->notify(this);
    }
    
    return true;
}

bool Greenhouse::removePlant(Plant* plant) {
    if(plant == nullptr){
        return false;
    }
    
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            if(plantGrid[i][j] == plant){
                plantGrid[i][j] = nullptr;
                currentNumberOfPlants--;
                
                std::cout << "Plant '" << plant->getName() << "' (ID: " << plant->getID() << ") removed from the greenhouse\n";
                
                if(mediator != nullptr){
                    mediator->notify(this);
                }
                
                return true;
            }
        }
    }
    
    return false;
}

Plant* Greenhouse::removePlantAt(int row, int col){
    if(row < 0 || row >= rows || col < 0 || col >= cols){
        return nullptr;
    }
    
    Plant* plant = plantGrid[row][col];
    
    if(plant != nullptr){
        plantGrid[row][col] = nullptr;
        currentNumberOfPlants--;
        
        std::cout << "Plant removed from greenhouse at (" << row << "," << col << ")\n";
        
        if(mediator != nullptr){
            mediator->notify(this);
        }
    }
    
    return plant;
}

Plant* Greenhouse::findPlant(std::string plantName){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){

            Plant* plant = plantGrid[i][j];

            if(plant != nullptr && (plant->getName() == plantName)){
                return plant;
            }
        }
    }

    return nullptr;
}

Plant* Greenhouse::getPlantAt(int row, int col)const{
    if(row < 0 || row >= rows || col < 0 || col >= cols){
        return nullptr;
    }
    return plantGrid[row][col];
}

bool Greenhouse::hasPlant(std::string plantName)const{
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            Plant* plant = plantGrid[i][j];

            if(plant != nullptr && (plant->getName() == plantName)){
                return true;
            }
        }
    }

    return false;
}

std::vector<Plant*> Greenhouse::getAllPlants()const{
    std::vector<Plant*> allPlants;
    
    for(int i = 0; i < rows; i++){ // flattening grid into a vector
        for(int j = 0; j < cols; j++){

            if(plantGrid[i][j] != nullptr){
                allPlants.push_back(plantGrid[i][j]);
            }
        }
    }
    
    return allPlants;
}

int Greenhouse::getNumberOfPlants()const{
    return currentNumberOfPlants;
}

int Greenhouse::getCapacity()const{
    return capacity;
}

bool Greenhouse::isFull()const{
    return currentNumberOfPlants >= capacity;
}

std::vector<Plant*> Greenhouse::getPlantsInRow(int row)const{
    std::vector<Plant*> rowPlants;
    
    if(row < 0 || row >= rows){
        return rowPlants;
    }
    
    for(int j = 0; j < cols; j++){

        if(plantGrid[row][j] != nullptr){
            rowPlants.push_back(plantGrid[row][j]);
        }
    }
    
    return rowPlants;
}

std::vector<Plant*> Greenhouse::getPlantsInColumn(int col)const{
    std::vector<Plant*> colPlants;
    
    if(col < 0 || col >= cols){
        return colPlants;
    }
    
    for(int i = 0; i < rows; i++){

        if(plantGrid[i][col] != nullptr){
            colPlants.push_back(plantGrid[i][col]);
        }
    }
    
    return colPlants;
}

int Greenhouse::getRows()const{
    return rows;
}

int Greenhouse::getColumns()const{
    return cols;
}

bool Greenhouse::isPositionEmpty(int row, int col)const{
    if(row < 0 || row >= rows || col < 0 || col >= cols){
        return false;
    }
    
    return plantGrid[row][col] == nullptr;
}