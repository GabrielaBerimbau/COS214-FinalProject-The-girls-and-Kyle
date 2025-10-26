#include "include/Plant.h"
#include "include/PlantObserver.h"
#include "include/CareStrategy.h"
#include "include/PlantState.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Plant::Plant(const std::string& name, const std::string& id, 
             CareStrategy* careStrategy, PlantState* initialState)
    : plantName(name), plantID(id), strategy(careStrategy), state(initialState),
      age(0), waterLevel(100), sunlightExposure(50), nutrientLevel(100),
      healthLevel(100), readyForSale(false), price(0.0) {
}

Plant::~Plant() {
    if (strategy != nullptr) {
        delete strategy;
        strategy = nullptr;
    }
    if (state != nullptr) {
        delete state;
        state = nullptr;
    }
    observers.clear();
}

void Plant::performCare() {
    if (strategy != nullptr) {
        strategy->performCare(this);
    }
}

void Plant::setStrategy(CareStrategy* newStrategy) {
    if (strategy != nullptr) {
        delete strategy;
    }
    strategy = newStrategy;
}

CareStrategy* Plant::getStrategy() const {
    return strategy;
}

PlantState* Plant::getState() const {
    return state;
}

void Plant::setState(PlantState* newState) {
    if (state != nullptr && state != newState) {
        delete state;
    }
    state = newState;
    if (state != nullptr) {
        std::string stateName = state->getStateName();
        if (stateName == "Mature" || stateName == "Flowering") {
            readyForSale = true;
        } else if (stateName == "Dead") {
            readyForSale = false;
        } else {
            readyForSale = false;
        }
    }
}

void Plant::attach(PlantObserver* observer) {
    if (observer == nullptr) {
        return;
    }
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it == observers.end()) {
        observers.push_back(observer);
    }
}

void Plant::detach(PlantObserver* observer) {
    if (observer == nullptr) {
        return;
    }
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void Plant::notify() {
    for (PlantObserver* observer : observers) {
        if (observer != nullptr) {
            observer->update(this);
        }
    }
}

std::string Plant::getName() const {
    return plantName;
}

std::string Plant::getID() const {
    return plantID;
}

int Plant::getAge() const {
    return age;
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
    if (newPrice >= 0) {
        price = newPrice;
    }
}

void Plant::incrementAge() {
    age++;
}

void Plant::dailyUpdate() {
    incrementAge();
    waterLevel -= 10;
    if (waterLevel < 0) waterLevel = 0;
    nutrientLevel -= 5;
    if (nutrientLevel < 0) nutrientLevel = 0;
    updateHealth();
    notify();
    if (state != nullptr) {
        state->handleChange(this);
    }
}

void Plant::updateCondition() {
    waterLevel -= 5;
    if (waterLevel < 0) waterLevel = 0;
    nutrientLevel -= 3;
    if (nutrientLevel < 0) nutrientLevel = 0;
    updateHealth();
    notify();
}

std::string Plant::toString() const {
    std::ostringstream output;
    output << "Plant: " << plantName << "\n"
           << "ID: " << plantID << "\n"
           << "Age: " << age << " days\n"
           << "State: " << (state ? state->getStateName() : "Unknown") << "\n"
           << "Water Level: " << waterLevel << "%\n"
           << "Nutrient Level: " << nutrientLevel << "%\n"
           << "Sunlight Exposure: " << sunlightExposure << "%\n"
           << "Health: " << healthLevel << "%\n"
           << "Ready for Sale: " << (readyForSale ? "Yes" : "No") << "\n"
           << "Price: R" << price;
    return output.str();
}