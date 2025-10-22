#ifndef PLANT_H
#define PLANT_H

#include "PlantObserverSubject.h"
#include "CareStrategy.h"
#include "PlantState.h"
#include <string>


class Plant : public PlantObserverSubject{
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
        std::string season; 


    public: 
        Plant(const std::string& name, const std::string& id, 
          CareStrategy* careStrategy, PlantState* initialState);

        ~Plant();

        virtual void performCare();

        PlantState* getState() const;

        void setState(PlantState* newState);

        void setStrategy(CareStrategy* newStrategy);

        std::string getID() const;

        int getAge();

        void incrementAge();

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

        double getPrice() const;

        void setPrice(double newPrice);

        std::string getSeason() const;

        void setSeason(const std::string& newSeason);

        /*
        daily plant growth and care needs, 
        Decreases water/nutrient levels, checks state transitions,
        and notifies observers of any changes.
        */
        void dailyUpdate(); 

        std::string toString() const;
};


#endif