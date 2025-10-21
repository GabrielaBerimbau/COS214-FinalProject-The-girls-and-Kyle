#ifndef ALOE_H
#define ALOE_H

#include "Succulent.h"

class Aloe : public Succulent {
    private:
        bool hasMedicinalProperties;    
        int gelContentLevel;            
        std::string aloeVariety;        
        int numberOfLeaves;             
        bool isHarvested;               

    public:
        Aloe(const std::string& id, CareStrategy* careStrategy,
             PlantState* initialState, const std::string& variety = "Vera");

        ~Aloe();

        bool hasMedicinalUse() const;

        void setMedicinalProperties(bool medicinal);

        int getGelContentLevel() const;

        void setGelContentLevel(int level);

        std::string getAloeVariety() const;

        int getNumberOfLeaves() const;

        void setNumberOfLeaves(int leaves);

        void addLeaf();

        bool wasRecentlyHarvested() const;

        void harvestGel();

        std::string toString() const override;
};

#endif