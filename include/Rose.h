#ifndef ROSE_H
#define ROSE_H

#include "Flower.h"

class Rose : public Flower {
    private:
        bool hasThorns;                 
        std::string roseType;           
        int bloomSize;                  
        bool repeatBloomer;             
        int pruneCount;                 

    public:
        Rose(const std::string& id, CareStrategy* careStrategy,
         PlantState* initialState, const std::string& roseColor,
         const std::string& type = "Hybrid Tea");

        ~Rose();

        bool getHasThorns() const;
        
        std::string getRoseType() const;
        
        int getBloomSize() const;
    
        void setBloomSize(int size);
    
        bool isRepeatBloomer() const;
        
        void pruneRose();
        
        int getPruneCount() const;
    
        bool checkForDisease() const;
        
        void treatDisease();

        std::string toString() const override;
        
};

#endif