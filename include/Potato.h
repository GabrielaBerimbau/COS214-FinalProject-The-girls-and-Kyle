#ifndef POTATO_H
#define POTATO_H

#include "Vegetable.h"

class Potato : public Vegetable {
    private:
        std::string potatoVariety;      
        int tuberCount;                 
        int averageTuberSize;           
        bool needsHilling;              
        int hillCount;                  
        bool hasFlowered;               

    public:
        Potato(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety = "Russet",
           const std::string& skin = "brown");

        ~Potato();

        std::string getPotatoVariety() const;

        int getTuberCount() const;
    
        void setTuberCount(int count);
        
        void formTubers();

        int getAverageTuberSize() const;
    
        void setAverageTuberSize(int size);
        
        bool requiresHilling() const;
        
        void hillPlant();
        
        int getHillCount() const;

        bool hasProducedFlowers() const;
    
        void setHasFlowered(bool flowered);
        
        bool isReadyForHarvest() const;
        
        int harvest() override;

        bool checkForBlight() const;
    
        std::string toString() const override;

};

#endif