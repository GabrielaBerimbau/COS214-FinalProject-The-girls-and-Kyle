#ifndef RADISH_H
#define RADISH_H

#include "Vegetable.h"

class Radish : public Vegetable {
    private:
        std::string radishVariety;      
        bool isCrunchyTexture;          
        int rootDepth;                  

    public:
        Radish(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& variety = "Cherry Belle",
           const std::string& color = "red");
    
        ~Radish();
        
        std::string getRadishVariety() const;

        bool isCrunchy() const;

        int getRootDepth() const;
    
        bool isReadyForHarvest() const;
        
        int harvest() override;
    
        //woody = inedibile
        bool isWoody() const;
        
        std::string toString() const override;

};

#endif