#ifndef CACTUS_H
#define CACTUS_H

#include "Succulent.h"

class Cactus : public Succulent {
    private:
        int spineCount;                 
        std::string spineColor;         
        std::string cactusShape;    
        bool canProduceFlowers;        
        bool isCurrentlyFlowering;     
        std::string cactusVariety;     

    public:
        Cactus(const std::string& id, CareStrategy* careStrategy,
               PlantState* initialState, const std::string& shape,
               const std::string& variety = "Saguaro");

        ~Cactus();

        std::string toString() const override;
};

#endif