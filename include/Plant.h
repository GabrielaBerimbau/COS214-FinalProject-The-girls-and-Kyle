#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <vector>

#include "CareStrategy.h"
#include "PlantState.h"
#include "PlantObserver.h"

class Plant {
private:
    CareStrategy* strategy;
    PlantState* state;
    std::string plantName;
    std::string plantID;
    int age;
    int waterLevel;
    int sunlightExposure;
    int nutrientLevel;
    int healthLevel;
    bool readyForSale;
    double price;
    std::vector<PlantObserver*> observers;
    std::vector<PlantObserver*> ownedObservers;

public:
    Plant(const std::string& name, const std::string& id, 
          CareStrategy* careStrategy, PlantState* initialState);
    Plant(const Plant& other);
    virtual ~Plant();

    void performCare();
    PlantState* getState() const;
    void setState(PlantState* newState);
    void setStrategy(CareStrategy* newStrategy);
    CareStrategy* getStrategy() const;
    
    void attach(PlantObserver* observer);
    void detach(PlantObserver* observer);
    void notify();
    void addOwnedObserver(PlantObserver* observer); 

    std::string getName() const;
    std::string getID() const;
    int getAge() const;
    int getWaterLevel() const;
    void setWaterLevel(int level);
    int getSunlightExposure() const;
    void setSunlightExposure(int hours);
    int getNutrientLevel() const;
    void setNutrientLevel(int level);
    int getHealthLevel() const;
    void updateHealth();
    bool isReadyForSale() const;
    void setReadyForSale(bool ready);
    virtual double getPrice() const;
    void setPrice(double newPrice);
    
    void incrementAge();
    virtual void dailyUpdate();
    void updateCondition();
    
    virtual std::string toString() const;
    virtual std::string description() const;


    CareStrategy* getCareStrategy() const;
};

#endif