/**
 * @file Plant.h
 * @author Kyle McCalgan
 * @brief Declares the Plant base class for the plant nursery system.
 *
 * This class serves as the abstract base for all plant types in the system,
 * implementing the State, Strategy, and Observer design patterns to manage
 * plant lifecycle, care requirements, and state changes.
 */
#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <vector>

#include "CareStrategy.h"
#include "PlantState.h"
#include "PlantObserver.h"

/**
 * @class Plant
 * @brief Base class representing a plant in the nursery system.
 *
 * This class manages plant attributes, lifecycle states, care strategies,
 * and observer notifications. It uses the Strategy pattern for care behavior,
 * State pattern for lifecycle management, and Observer pattern for notifications.
 */
class Plant {
private:
    CareStrategy* strategy;
    PlantState* state;
    std::string plantName;
    std::string plantID;
    int age;
    int waterLevel;
    int sunlightExposure;
    int nutrientLevel;
    int healthLevel;
    bool readyForSale;
    double price;
    std::vector<PlantObserver*> observers;
    std::vector<PlantObserver*> ownedObservers;

public:
    /**
     * @brief Constructs a new Plant object.
     * @param name The name of the plant.
     * @param id Unique identifier for the plant.
     * @param careStrategy Pointer to the care strategy for this plant.
     * @param initialState Pointer to the initial state of the plant.
     */
    Plant(const std::string& name, const std::string& id,
          CareStrategy* careStrategy, PlantState* initialState);

    /**
     * @brief Copy constructor.
     * @param other The plant object to copy from.
     */
    Plant(const Plant& other);

    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of observers and owned resources.
     */
    virtual ~Plant();

    /**
     * @brief Performs care on the plant using the current care strategy.
     */
    void performCare();

    /**
     * @brief Gets the current state of the plant.
     * @return Pointer to the current PlantState object.
     */
    PlantState* getState() const;

    /**
     * @brief Sets a new state for the plant.
     * @param newState Pointer to the new PlantState object.
     */
    void setState(PlantState* newState);

    /**
     * @brief Sets a new care strategy for the plant.
     * @param newStrategy Pointer to the new CareStrategy object.
     */
    void setStrategy(CareStrategy* newStrategy);

    /**
     * @brief Gets the current care strategy.
     * @return Pointer to the current CareStrategy object.
     */
    CareStrategy* getStrategy() const;

    /**
     * @brief Attaches an observer to the plant.
     * @param observer Pointer to the PlantObserver to attach.
     */
    void attach(PlantObserver* observer);

    /**
     * @brief Detaches an observer from the plant.
     * @param observer Pointer to the PlantObserver to detach.
     */
    void detach(PlantObserver* observer);

    /**
     * @brief Notifies all attached observers of state changes.
     */
    void notify();

    /**
     * @brief Adds an observer to the owned observers list.
     * @param observer Pointer to the PlantObserver that will be owned by this plant.
     */
    void addOwnedObserver(PlantObserver* observer);

    /**
     * @brief Gets the name of the plant.
     * @return The plant's name as a string.
     */
    std::string getName() const;

    /**
     * @brief Gets the unique identifier of the plant.
     * @return The plant's ID as a string.
     */
    std::string getID() const;

    /**
     * @brief Gets the age of the plant.
     * @return The plant's age as an integer.
     */
    int getAge() const;

    /**
     * @brief Gets the current water level of the plant.
     * @return The water level as an integer.
     */
    int getWaterLevel() const;

    /**
     * @brief Sets the water level of the plant.
     * @param level The new water level.
     */
    void setWaterLevel(int level);

    /**
     * @brief Gets the current sunlight exposure of the plant.
     * @return The sunlight exposure as an integer.
     */
    int getSunlightExposure() const;

    /**
     * @brief Sets the sunlight exposure of the plant.
     * @param hours The new sunlight exposure in hours.
     */
    void setSunlightExposure(int hours);

    /**
     * @brief Gets the current nutrient level of the plant.
     * @return The nutrient level as an integer.
     */
    int getNutrientLevel() const;

    /**
     * @brief Sets the nutrient level of the plant.
     * @param level The new nutrient level.
     */
    void setNutrientLevel(int level);

    /**
     * @brief Gets the current health level of the plant.
     * @return The health level as an integer.
     */
    int getHealthLevel() const;

    /**
     * @brief Updates the health level based on current conditions.
     */
    void updateHealth();

    /**
     * @brief Checks if the plant is ready for sale.
     * @return True if ready for sale, false otherwise.
     */
    bool isReadyForSale() const;

    /**
     * @brief Sets whether the plant is ready for sale.
     * @param ready Boolean indicating sale readiness.
     */
    void setReadyForSale(bool ready);

    /**
     * @brief Gets the price of the plant.
     * @return The price as a double.
     */
    virtual double getPrice() const;

    /**
     * @brief Sets the price of the plant.
     * @param newPrice The new price value.
     */
    void setPrice(double newPrice);

    /**
     * @brief Increments the age of the plant by one unit.
     */
    void incrementAge();

    /**
     * @brief Performs daily update operations on the plant.
     * Updates age, conditions, and state transitions.
     */
    virtual void dailyUpdate();

    /**
     * @brief Updates the plant's condition based on current levels.
     */
    void updateCondition();

    /**
     * @brief Converts plant information to a string representation.
     * @return String containing plant details.
     */
    virtual std::string toString() const;

    /**
     * @brief Provides a description of the plant.
     * @return String description of the plant.
     */
    virtual std::string description() const;

    /**
     * @brief Gets the care strategy for the plant.
     * @return Pointer to the CareStrategy object.
     */
    CareStrategy* getCareStrategy() const;
};

#endif