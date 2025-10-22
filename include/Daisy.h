#ifndef DAISY_H
#define DAISY_H

#include "Flower.h"

class Daisy : public Flower {
    private:
        bool hasYellowCenter;           
        int numberOfBlooms;             
        std::string variety;            

    public:
        Daisy(const std::string& id, CareStrategy* careStrategy,
          PlantState* initialState, const std::string& petalColor,
          const std::string& varietyType = "Common");
    
    ~Daisy();
    
    bool hasYellowCenterDisk() const;
    
    void setYellowCenter(bool hasYellow);
   
    int getNumberOfBlooms() const;
   
    void setNumberOfBlooms(int blooms);
    
    void addBloom();
    
    std::string getVariety() const;
   
    void deadhead();
    
    std::string toString() const override;

};

#endif