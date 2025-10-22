#ifndef SUCCULENT_H
#define SUCCULENT_H

#include "Plant.h"
#include <string>

class Succulent : public Plant {
    protected:
        int waterRetention;           
        bool hasThickLeaves;          
        std::string succulentType;    
        int droughtTolerance;         
        bool isSpiny;                 

    public:
        Succulent(const std::string& name, const std::string& id,
                  CareStrategy* careStrategy, PlantState* initialState,
                  int retention, bool thickLeaves);

        virtual ~Succulent();

        int getWaterRetention() const;

        void setWaterRetention(int retention);

        bool hasThickLeavesCharacteristic() const;

        std::string getSucculentType() const;

        int getDroughtTolerance() const;

        void setDroughtTolerance(int tolerance);

        bool isSpinyPlant() const;

        void setSpiny(bool spiny);

        virtual std::string toString() const;
};

#endif