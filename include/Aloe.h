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
        
        std::string toString() const override;
};

#endif