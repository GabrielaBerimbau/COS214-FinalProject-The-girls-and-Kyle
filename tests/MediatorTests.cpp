#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

#include "include/NurseryMediator.h"
#include "include/NurseryCoordinator.h"
#include "include/Colleague.h"
#include "include/Person.h"
#include "include/SalesFloor.h"
#include "include/Greenhouse.h"
#include "include/Customer.h"
#include "include/DerivedCustomers.h"
#include "include/Plant.h"
#include "include/FlowerCareStrategy.h"
#include "include/MatureState.h"

// ============ Mediator Pattern Test Fixture ============

class MediatorTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    SalesFloor* salesFloor;
    Greenhouse* greenhouse;
    RegularCustomer* customer;
    Plant* testPlant1;
    Plant* testPlant2;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        salesFloor = new SalesFloor(mediator, 3, 3);
        greenhouse = new Greenhouse(mediator, 2, 2);
        
        mediator->registerColleague(salesFloor);
        mediator->registerColleague(greenhouse);
        
        // Create test plants
        testPlant1 = new Plant("Rose", "R001", 
                              new FlowerCareStrategy(), 
                              new MatureState());
        testPlant1->setPrice(50.0);
        testPlant1->setReadyForSale(true);
        
        testPlant2 = new Plant("Tulip", "T001", 
                              new FlowerCareStrategy(), 
                              new MatureState());
        testPlant2->setPrice(30.0);
        testPlant2->setReadyForSale(true);
        
        customer = new RegularCustomer();
        customer->setMediator(mediator);
        customer->setName("Test Customer");
        customer->setId("CUST-001");
        mediator->registerColleague(customer);
    }
    
    void TearDown() override {
        delete customer;
        delete salesFloor;
        delete greenhouse;
        delete mediator;
        // Plants are deleted by their containers
    }
};

// ============ NurseryMediator Tests ============

TEST_F(MediatorTest, MediatorCreation) {
    EXPECT_NE(mediator, nullptr);
}

TEST_F(MediatorTest, RegisterColleague) {
    NurseryMediator* newMediator = new NurseryMediator();
    SalesFloor* newFloor = new SalesFloor(newMediator, 2, 2);
    
    EXPECT_NO_THROW(newMediator->registerColleague(newFloor));
    
    delete newFloor;
    delete newMediator;
}

TEST_F(MediatorTest, RegisterNullColleague) {
    EXPECT_NO_THROW(mediator->registerColleague(nullptr));
}

TEST_F(MediatorTest, RemoveColleague) {
    NurseryMediator* newMediator = new NurseryMediator();
    SalesFloor* newFloor = new SalesFloor(newMediator, 2, 2);
    
    newMediator->registerColleague(newFloor);
    EXPECT_NO_THROW(newMediator->removeColleague(newFloor));
    
    delete newFloor;
    delete newMediator;
}

TEST_F(MediatorTest, NotifyColleague) {
    EXPECT_NO_THROW(mediator->notify(salesFloor));
    EXPECT_NO_THROW(mediator->notify(greenhouse));
}

TEST_F(MediatorTest, RequestPlantFromStaff) {
    salesFloor->addPlantToDisplay(testPlant1, 0, 0);
    
    Plant* found = mediator->requestPlantFromStaff("Rose");
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getName(), "Rose");
}

TEST_F(MediatorTest, RequestNonexistentPlant) {
    Plant* found = mediator->requestPlantFromStaff("Cactus");
    EXPECT_EQ(found, nullptr);
}

TEST_F(MediatorTest, StaffChecksGreenhouse) {
    greenhouse->addPlant(testPlant1, 0, 0);
    
    bool hasPlant = mediator->staffChecksGreenHouse("Rose");
    EXPECT_TRUE(hasPlant);
    
    bool doesNotHave = mediator->staffChecksGreenHouse("Cactus");
    EXPECT_FALSE(doesNotHave);
}

TEST_F(MediatorTest, PlantUnavailableMessage) {
    std::string message = mediator->plantUnavailable();
    EXPECT_FALSE(message.empty());
    EXPECT_NE(message.find("unavailable"), std::string::npos);
}

TEST_F(MediatorTest, TransferPlantFromSalesFloor) {
    salesFloor->addPlantToDisplay(testPlant1, 0, 0);
    
    EXPECT_EQ(customer->getCartSize(), 0);
    
    bool success = mediator->transferPlantToCustomer("Rose", customer);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(customer->getCartSize(), 1);
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 0);
}

TEST_F(MediatorTest, TransferPlantFromGreenhouse) {
    greenhouse->addPlant(testPlant1, 0, 0);
    
    bool success = mediator->transferPlantToCustomer("Rose", customer);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(customer->getCartSize(), 1);
    EXPECT_EQ(greenhouse->getNumberOfPlants(), 0);
}

TEST_F(MediatorTest, TransferPlantNotReady) {
    testPlant1->setReadyForSale(false);
    greenhouse->addPlant(testPlant1, 0, 0);
    
    bool success = mediator->transferPlantToCustomer("Rose", customer);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(customer->getCartSize(), 0);
}

TEST_F(MediatorTest, TransferPlantFromPosition) {
    salesFloor->addPlantToDisplay(testPlant1, 1, 1);
    
    bool success = mediator->transferPlantFromPosition(1, 1, customer);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(customer->getCartSize(), 1);
    EXPECT_TRUE(salesFloor->isPositionEmpty(1, 1));
}

TEST_F(MediatorTest, TransferFromInvalidPosition) {
    bool success = mediator->transferPlantFromPosition(5, 5, customer);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(customer->getCartSize(), 0);
}

TEST_F(MediatorTest, TransferToNullCustomer) {
    salesFloor->addPlantToDisplay(testPlant1, 0, 0);
    
    bool success = mediator->transferPlantToCustomer("Rose", nullptr);
    
    EXPECT_FALSE(success);
}

TEST_F(MediatorTest, ReturnPlantToDisplay) {
    salesFloor->addPlantToDisplay(testPlant1, 0, 0);
    mediator->transferPlantToCustomer("Rose", customer);
    
    bool success = customer->returnPlantToSalesFloor(0);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 1);
    EXPECT_EQ(customer->getCartSize(), 0);
}

TEST_F(MediatorTest, ReturnNullPlant) {
    bool success = mediator->returnPlantToDisplay(nullptr);
    EXPECT_FALSE(success);
}

TEST_F(MediatorTest, StaffAddPlantToCustomerCart) {
    salesFloor->addPlantToDisplay(testPlant1, 0, 0);
    
    bool success = mediator->staffAddPlantToCustomerCart("Rose", customer);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(customer->getCartSize(), 1);
}

// ============ NurseryCoordinator Tests ============

class CoordinatorTest : public ::testing::Test {
protected:
    NurseryCoordinator* coordinator;
    SalesFloor* salesFloor;
    Greenhouse* greenhouse;
    Plant* testPlant;
    
    void SetUp() override {
        coordinator = new NurseryCoordinator();
        salesFloor = new SalesFloor(coordinator, 3, 3);
        greenhouse = new Greenhouse(coordinator, 2, 2);
        
        coordinator->registerColleague(salesFloor);
        coordinator->registerColleague(greenhouse);
        coordinator->setSalesFloor(salesFloor);
        coordinator->setGreenhouse(greenhouse);
        
        testPlant = new Plant("Rose", "R001", 
                             new FlowerCareStrategy(), 
                             new MatureState());
        testPlant->setPrice(50.0);
        testPlant->setReadyForSale(true);
    }
    
    void TearDown() override {
        delete salesFloor;
        delete greenhouse;
        delete coordinator;
        // Plant deleted by containers
    }
};

TEST_F(CoordinatorTest, CoordinatorCreation) {
    EXPECT_NE(coordinator, nullptr);
}

TEST_F(CoordinatorTest, SetSalesFloor) {
    NurseryCoordinator* newCoord = new NurseryCoordinator();
    SalesFloor* newFloor = new SalesFloor(newCoord, 2, 2);
    
    EXPECT_NO_THROW(newCoord->setSalesFloor(newFloor));
    
    delete newFloor;
    delete newCoord;
}

TEST_F(CoordinatorTest, SetGreenhouse) {
    NurseryCoordinator* newCoord = new NurseryCoordinator();
    Greenhouse* newGH = new Greenhouse(newCoord, 2, 2);
    
    EXPECT_NO_THROW(newCoord->setGreenhouse(newGH));
    
    delete newGH;
    delete newCoord;
}

TEST_F(CoordinatorTest, CoordinatePlantTransfer) {
    greenhouse->addPlant(testPlant, 0, 0);
    
    bool success = coordinator->coordinatePlantTransfer("Rose");
    
    EXPECT_TRUE(success);
    EXPECT_EQ(greenhouse->getNumberOfPlants(), 0);
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 1);
}

TEST_F(CoordinatorTest, CoordinateTransferNotReadyPlant) {
    testPlant->setReadyForSale(false);
    greenhouse->addPlant(testPlant, 0, 0);
    
    bool success = coordinator->coordinatePlantTransfer("Rose");
    
    EXPECT_FALSE(success);
    EXPECT_EQ(greenhouse->getNumberOfPlants(), 1);
}

TEST_F(CoordinatorTest, CheckPlantRelocation) {
    greenhouse->addPlant(testPlant, 0, 0);
    
    EXPECT_NO_THROW(coordinator->checkPlantRelocation());
    
    // Plant should be moved to sales floor
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 1);
}

// ============ Colleague Tests ============

class ColleagueTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    SalesFloor* colleague;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        colleague = new SalesFloor(mediator, 2, 2);
    }
    
    void TearDown() override {
        delete colleague;
        delete mediator;
    }
};

TEST_F(ColleagueTest, ColleagueHasMediator) {
    EXPECT_EQ(colleague->getMediator(), mediator);
}

TEST_F(ColleagueTest, SetMediator) {
    NurseryMediator* newMediator = new NurseryMediator();
    
    colleague->setMediator(newMediator);
    EXPECT_EQ(colleague->getMediator(), newMediator);
    
    delete newMediator;
}

TEST_F(ColleagueTest, SetNullMediator) {
    EXPECT_NO_THROW(colleague->setMediator(nullptr));
}

// ============ SalesFloor Tests ============

class SalesFloorTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    SalesFloor* salesFloor;
    Plant* plant1;
    Plant* plant2;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        salesFloor = new SalesFloor(mediator, 3, 3);
        
        plant1 = new Plant("Rose", "R001", 
                          new FlowerCareStrategy(), 
                          new MatureState());
        plant1->setPrice(50.0);
        
        plant2 = new Plant("Tulip", "T001", 
                          new FlowerCareStrategy(), 
                          new MatureState());
        plant2->setPrice(30.0);
    }
    
    void TearDown() override {
        delete salesFloor;
        delete mediator;
        // Plants deleted by sales floor
    }
};

TEST_F(SalesFloorTest, SalesFloorCreation) {
    EXPECT_EQ(salesFloor->getRows(), 3);
    EXPECT_EQ(salesFloor->getColumns(), 3);
    EXPECT_EQ(salesFloor->getCapacity(), 9);
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 0);
}

TEST_F(SalesFloorTest, AddPlantToDisplay) {
    bool success = salesFloor->addPlantToDisplay(plant1, 0, 0);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 1);
    EXPECT_FALSE(salesFloor->isPositionEmpty(0, 0));
}

TEST_F(SalesFloorTest, AddPlantToOccupiedPosition) {
    salesFloor->addPlantToDisplay(plant1, 0, 0);
    
    bool success = salesFloor->addPlantToDisplay(plant2, 0, 0);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 1);
}

TEST_F(SalesFloorTest, AddPlantToInvalidPosition) {
    bool success = salesFloor->addPlantToDisplay(plant1, 5, 5);
    
    EXPECT_FALSE(success);
}

TEST_F(SalesFloorTest, AddNullPlant) {
    bool success = salesFloor->addPlantToDisplay(nullptr, 0, 0);
    
    EXPECT_FALSE(success);
}

TEST_F(SalesFloorTest, RemovePlantFromDisplay) {
    salesFloor->addPlantToDisplay(plant1, 0, 0);
    
    salesFloor->removePlantFromDisplay(plant1);
    
    EXPECT_EQ(salesFloor->getNumberOfPlants(), 0);
    EXPECT_TRUE(salesFloor->isPositionEmpty(0, 0));
}

TEST_F(SalesFloorTest, RemovePlantAt) {
    salesFloor->addPlantToDisplay(plant1, 1, 1);
    
    Plant* removed = salesFloor->removePlantAt(1, 1);
    
    EXPECT_NE(removed, nullptr);
    EXPECT_EQ(removed, plant1);
    EXPECT_TRUE(salesFloor->isPositionEmpty(1, 1));
}

TEST_F(SalesFloorTest, GetPlantAt) {
    salesFloor->addPlantToDisplay(plant1, 0, 0);
    
    Plant* retrieved = salesFloor->getPlantAt(0, 0);
    
    EXPECT_EQ(retrieved, plant1);
}

TEST_F(SalesFloorTest, GetPlantAtEmptyPosition) {
    Plant* retrieved = salesFloor->getPlantAt(0, 0);
    
    EXPECT_EQ(retrieved, nullptr);
}

TEST_F(SalesFloorTest, IsFull) {
    EXPECT_FALSE(salesFloor->isFull());
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Plant* p = new Plant("Plant", "P" + std::to_string(i*3+j), 
                               new FlowerCareStrategy(), 
                               new MatureState());
            salesFloor->addPlantToDisplay(p, i, j);
        }
    }
    
    EXPECT_TRUE(salesFloor->isFull());
}

TEST_F(SalesFloorTest, GetDisplayPlants) {
    salesFloor->addPlantToDisplay(plant1, 0, 0);
    salesFloor->addPlantToDisplay(plant2, 0, 1);
    
    std::vector<Plant*> plants = salesFloor->getDisplayPlants();
    
    EXPECT_EQ(plants.size(), 2);
}

TEST_F(SalesFloorTest, GetPlantsInRow) {
    salesFloor->addPlantToDisplay(plant1, 0, 0);
    salesFloor->addPlantToDisplay(plant2, 0, 1);
    
    std::vector<Plant*> rowPlants = salesFloor->getPlantsInRow(0);
    
    EXPECT_EQ(rowPlants.size(), 2);
}

TEST_F(SalesFloorTest, GetPlantsInColumn) {
    salesFloor->addPlantToDisplay(plant1, 0, 0);
    salesFloor->addPlantToDisplay(plant2, 1, 0);
    
    std::vector<Plant*> colPlants = salesFloor->getPlantsInColumn(0);
    
    EXPECT_EQ(colPlants.size(), 2);
}

TEST_F(SalesFloorTest, AddCustomer) {
    RegularCustomer* customer = new RegularCustomer();
    
    EXPECT_NO_THROW(salesFloor->addCustomer(customer));
    EXPECT_EQ(salesFloor->getCurrentCustomers().size(), 1);
    
    delete customer;
}

TEST_F(SalesFloorTest, RemoveCustomer) {
    RegularCustomer* customer = new RegularCustomer();
    
    salesFloor->addCustomer(customer);
    salesFloor->removeCustomer(customer);
    
    EXPECT_EQ(salesFloor->getCurrentCustomers().size(), 0);
    
    delete customer;
}

// ============ Greenhouse Tests ============

class GreenhouseTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    Greenhouse* greenhouse;
    Plant* plant1;
    Plant* plant2;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        greenhouse = new Greenhouse(mediator, 2, 2);
        
        plant1 = new Plant("Rose", "R001", 
                          new FlowerCareStrategy(), 
                          new MatureState());
        plant1->setReadyForSale(false);
        
        plant2 = new Plant("Tulip", "T001", 
                          new FlowerCareStrategy(), 
                          new MatureState());
        plant2->setReadyForSale(false);
    }
    
    void TearDown() override {
        delete greenhouse;
        delete mediator;
        // Plants deleted by greenhouse
    }
};

TEST_F(GreenhouseTest, GreenhouseCreation) {
    EXPECT_EQ(greenhouse->getRows(), 2);
    EXPECT_EQ(greenhouse->getColumns(), 2);
    EXPECT_EQ(greenhouse->getCapacity(), 4);
    EXPECT_EQ(greenhouse->getNumberOfPlants(), 0);
}

TEST_F(GreenhouseTest, AddPlant) {
    bool success = greenhouse->addPlant(plant1, 0, 0);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(greenhouse->getNumberOfPlants(), 1);
}

TEST_F(GreenhouseTest, AddPlantToOccupiedPosition) {
    greenhouse->addPlant(plant1, 0, 0);
    
    bool success = greenhouse->addPlant(plant2, 0, 0);
    
    EXPECT_FALSE(success);
}

TEST_F(GreenhouseTest, AddNullPlant) {
    bool success = greenhouse->addPlant(nullptr, 0, 0);
    
    EXPECT_FALSE(success);
}

TEST_F(GreenhouseTest, RemovePlant) {
    greenhouse->addPlant(plant1, 0, 0);
    
    bool success = greenhouse->removePlant(plant1);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(greenhouse->getNumberOfPlants(), 0);
}

TEST_F(GreenhouseTest, RemovePlantAt) {
    greenhouse->addPlant(plant1, 0, 0);
    
    Plant* removed = greenhouse->removePlantAt(0, 0);
    
    EXPECT_NE(removed, nullptr);
    EXPECT_EQ(removed, plant1);
}

TEST_F(GreenhouseTest, FindPlant) {
    greenhouse->addPlant(plant1, 0, 0);
    
    Plant* found = greenhouse->findPlant("Rose");
    
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getName(), "Rose");
}

TEST_F(GreenhouseTest, FindNonexistentPlant) {
    Plant* found = greenhouse->findPlant("Cactus");
    
    EXPECT_EQ(found, nullptr);
}

TEST_F(GreenhouseTest, HasPlant) {
    greenhouse->addPlant(plant1, 0, 0);
    
    EXPECT_TRUE(greenhouse->hasPlant("Rose"));
    EXPECT_FALSE(greenhouse->hasPlant("Cactus"));
}

TEST_F(GreenhouseTest, GetPlantAt) {
    greenhouse->addPlant(plant1, 0, 0);
    
    Plant* retrieved = greenhouse->getPlantAt(0, 0);
    
    EXPECT_EQ(retrieved, plant1);
}

TEST_F(GreenhouseTest, GetAllPlants) {
    greenhouse->addPlant(plant1, 0, 0);
    greenhouse->addPlant(plant2, 0, 1);
    
    std::vector<Plant*> plants = greenhouse->getAllPlants();
    
    EXPECT_EQ(plants.size(), 2);
}

TEST_F(GreenhouseTest, IsFull) {
    EXPECT_FALSE(greenhouse->isFull());
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Plant* p = new Plant("Plant", "P" + std::to_string(i*2+j), 
                               new FlowerCareStrategy(), 
                               new MatureState());
            greenhouse->addPlant(p, i, j);
        }
    }
    
    EXPECT_TRUE(greenhouse->isFull());
}

TEST_F(GreenhouseTest, IsPositionEmpty) {
    EXPECT_TRUE(greenhouse->isPositionEmpty(0, 0));
    
    greenhouse->addPlant(plant1, 0, 0);
    
    EXPECT_FALSE(greenhouse->isPositionEmpty(0, 0));
}

TEST_F(GreenhouseTest, GetPlantsInRow) {
    greenhouse->addPlant(plant1, 0, 0);
    greenhouse->addPlant(plant2, 0, 1);
    
    std::vector<Plant*> rowPlants = greenhouse->getPlantsInRow(0);
    
    EXPECT_EQ(rowPlants.size(), 2);
}

TEST_F(GreenhouseTest, GetPlantsInColumn) {
    greenhouse->addPlant(plant1, 0, 0);
    greenhouse->addPlant(plant2, 1, 0);
    
    std::vector<Plant*> colPlants = greenhouse->getPlantsInColumn(0);
    
    EXPECT_EQ(colPlants.size(), 2);
}