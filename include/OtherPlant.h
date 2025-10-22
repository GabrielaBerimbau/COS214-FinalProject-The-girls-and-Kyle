#ifndef OTHERPLANT_H
#define OTHERPLANT_H

#include "Plant.h"
#include <string>

class OtherPlant : public Plant {
    protected:
        std::string specialCharacteristic;  
        bool isCarnivorousPlant;           
        std::string habitatType;           
        int maintenanceDifficulty;         
        bool requiresSpecialCare;          

    public:
        OtherPlant(const std::string& name, const std::string& id,
                   CareStrategy* careStrategy, PlantState* initialState,
                   const std::string& characteristic, bool carnivorous);

        virtual ~OtherPlant();

        std::string getSpecialCharacteristic() const;

        void setSpecialCharacteristic(const std::string& characteristic);

        bool isCarnivorous() const;

        std::string getHabitatType() const;

        void setHabitatType(const std::string& habitat);

        int getMaintenanceDifficulty() const;

        void setMaintenanceDifficulty(int difficulty);

        bool needsSpecialCare() const;

        void setRequiresSpecialCare(bool special);

        virtual std::string toString() const;
};

#endif