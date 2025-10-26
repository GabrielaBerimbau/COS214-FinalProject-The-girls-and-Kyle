#include "Plant.h"
#include <iostream>
#include <sstream>

Plant::Plant(const std::string& name, const std::string& id, 
             CareStrategy* careStrategy, PlantState* initialState)
    : plantName(name), plantID(id), strategy(careStrategy), state(initialState),
      age(0), waterLevel(100), sunlightExposure(50), nutrientLevel(100),
      healthLevel(100), readyForSale(false), price(0.0), season("Spring") {
    
    std::cout << "Plant created: " << plantName << " (ID: " << plantID << ")" << std::endl;
}

Plant::~Plant() {
    delete strategy;
    
    std::cout << "Plant destroyed: " << plantName << std::endl;
}

void Plant::performCare() {
    if (strategy) {
        strategy->water(this);
    } else {
        std::cout << "No care strategy assigned to plant: " << plantName << std::endl;
    }
}

PlantState* Plant::getState() const {
    return state;
}

void Plant::setState(PlantState* newState) {
    state = newState;
    if (state) {
        std::string stateName = state->getStateName();
        if (stateName == "Mature" || stateName == "Flowering") {
            readyForSale = true;
        } else {
            readyForSale = false;
        }
    }
}

void Plant::setStrategy(CareStrategy* newStrategy) {
    if (strategy) {
        delete strategy;
    }
    strategy = newStrategy;
}

std::string Plant::getID() const {
    return plantID;
}

int Plant::getAge() {
    return age;
}

void Plant::incrementAge() {
    age++;
}

int Plant::getWaterLevel() const {
    return waterLevel;
}

void Plant::setWaterLevel(int level) {
    waterLevel = level;
    if (waterLevel < 0) waterLevel = 0;
    if (waterLevel > 100) waterLevel = 100;
}

int Plant::getSunlightExposure() const {
    return sunlightExposure;
}

void Plant::setSunlightExposure(int hours) {
    sunlightExposure = hours;
    if (sunlightExposure < 0) sunlightExposure = 0;
    if (sunlightExposure > 100) sunlightExposure = 100;
}

int Plant::getNutrientLevel() const {
    return nutrientLevel;
}

void Plant::setNutrientLevel(int level) {
    nutrientLevel = level;
    if (nutrientLevel < 0) nutrientLevel = 0;
    if (nutrientLevel > 100) nutrientLevel = 100;
}

int Plant::getHealthLevel() const {
    return healthLevel;
}

void Plant::updateHealth() {
    healthLevel = (waterLevel + nutrientLevel + sunlightExposure) / 3;
    
    if (healthLevel < 20) {
        std::cout << "WARNING: " << plantName << " is in poor health!" << std::endl;
    }
}

bool Plant::isReadyForSale() const {
    return readyForSale;
}

void Plant::setReadyForSale(bool ready) {
    readyForSale = ready;
}

double Plant::getPrice() const {
    return price;
}

void Plant::setPrice(double newPrice) {
    price = newPrice;
}

std::string Plant::getSeason() const {
    return season;
}

void Plant::setSeason(const std::string& newSeason) {
    season = newSeason;
}

void Plant::dailyUpdate() {
    incrementAge();
    
    waterLevel -= 10;
    if (waterLevel < 0) waterLevel = 0;
    
    nutrientLevel -= 5;
    if (nutrientLevel < 0) nutrientLevel = 0;
    
   updateHealth();
    
    notify();
    
    if (state) {
        state->handleChange(this);
    }
    
    std::cout << "Daily update completed for " << plantName 
              << " (Age: " << age << " days, Health: " << healthLevel << "%)" << std::endl;
}

std::string Plant::toString() const {
    std::ostringstream output;
    output << "Plant: " << plantName << "\n"
        << "  ID: " << plantID << "\n"
        << "  Age: " << age << " days\n"
        << "  State: " << (state ? state->getStateName() : "Unknown") << "\n"
        << "  Water Level: " << waterLevel << "%\n"
        << "  Nutrient Level: " << nutrientLevel << "%\n"
        << "  Sunlight Exposure: " << sunlightExposure << "%\n"
        << "  Health: " << healthLevel << "%\n"
        << "  Ready for Sale: " << (readyForSale ? "Yes" : "No") << "\n"
        << "  Price: R" << price << "\n"
        << "  Season: " << season;
    
    return output.str();
}