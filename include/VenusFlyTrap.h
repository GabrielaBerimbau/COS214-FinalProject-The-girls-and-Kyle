#ifndef VENUSFLYTRAP_H
#define VENUSFLYTRAP_H

#include "OtherPlant.h"

class VenusFlyTrap : public OtherPlant {
    private:
        int numberOfTraps;              
        bool trapIsOpen;                
        int trapSensitivity;            
        int insectsCaptured;            
        int daysSinceLastFeeding;       
        std::string trapColor;          

    public:
        VenusFlyTrap(const std::string& id, CareStrategy* careStrategy,
                     PlantState* initialState, int initialTraps = 5);

        ~VenusFlyTrap();

        int getNumberOfTraps() const;

        void setNumberOfTraps(int traps);

        void addTrap();

        bool isTrapOpen() const;

        void openTrap();

        void closeTrap();

        int getTrapSensitivity() const;

        void setTrapSensitivity(int sensitivity);

        int getInsectsCaptured() const;

        void captureInsect();

        int getDaysSinceLastFeeding() const;

        void updateFeedingStatus();

        std::string getTrapColor() const;

        void setTrapColor(const std::string& color);

        std::string toString() const override;
};

#endif