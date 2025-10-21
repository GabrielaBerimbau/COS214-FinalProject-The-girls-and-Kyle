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

        int getSpineCount() const;

        void setSpineCount(int count);

        std::string getSpineColor() const;

        void setSpineColor(const std::string& color);

        std::string getCactusShape() const;

        bool canFlower() const;

        void setCanProduceFlowers(bool canFlower);

        bool isFlowering() const;

        void startFlowering();

        void endFlowering();

        std::string getCactusVariety() const;

        std::string toString() const override;
};

#endif