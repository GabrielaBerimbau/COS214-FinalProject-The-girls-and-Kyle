#ifndef VEGETABLE_H
#define VEGETABLE_H

#include "Plant.h"
#include <string>

class Vegetable : public Plant {
    protected:
        bool isEdible;                  
        int daysToHarvest;             
        std::string harvestSeason;      
        bool isReadyToHarvest;          
        int yieldAmount;                
        std::string vegetableType;      
        int pestResistance;  
    
    public:
        Vegetable(const std::string& name, const std::string& id,
              CareStrategy* careStrategy, PlantState* initialState,
              const std::string& vegType, const std::string& edible);

        virtual ~Vegetable();
    
        bool getIsEdible() const;
   
        int getDaysToHarvest() const;
    
        void decrementDaysToHarvest();
        
        std::string getHarvestSeason() const;
        
        void setHarvestSeason(const std::string& season);
        
        bool checkReadyToHarvest() const;
        
        void setReadyToHarvest(bool ready);
        
        int getYieldAmount() const;
        
        void setYieldAmount(int yield);
    
        std::string getVegetableType() const;
    
        int getPestResistance() const;
        
        void setPestResistance(int resistance);
        
        virtual int harvest();
        
        bool checkForPests() const;
        
        virtual std::string toString() const;


};

#endif