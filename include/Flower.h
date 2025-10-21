#ifndef FLOWER_H
#define FLOWER_H

#include "Plant.h"
#include <string>

class Flower : public Plant {
    protected:
        std::string color;              
        std::string bloomSeason;        
        bool isFragrant;               
        bool isCurrentlyBlooming;       
        int bloomDuration;              
        int daysUntilNextBloom;         
        std::string flowerType;  

    public:
        Flower(const std::string& name, const std::string& id,
           CareStrategy* careStrategy, PlantState* initialState,
           const std::string& flowerColor, bool fragrant);

        virtual ~Flower();

        std::string getColor() const;

        void setColor(const std::string& newColor);

        std::string getBloomSeason() const;

        void setBloomSeason(const std::string& season);

        bool getIsFragrant() const;

        bool isBlooming() const;

        void startBloom();

        void endBloom();

        int getDaysUntilNextBloom() const;

        void updateBloomStatus();

        std::string getFlowerType() const;

        virtual std::string toString() const;
};

#endif