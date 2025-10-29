#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>

class Customer;

/**
 * @enum RequestLevel
 * @brief Priority levels for requests in chain of responsibility
 */
enum class RequestLevel{
    LOW,      // handled by SalesAssistant
    MEDIUM,   // handled by FloorManager
    HIGH      // handled by NurseryOwner
};

/**
 * @class Request
 * @brief Represents a customer request to be processed through chain of responsibility
 */
class Request{
    private:
        std::string message;
        RequestLevel level;
        bool handled;
        Customer* requestingCustomer;
        
        /**
         * @brief Extract keywords from sentence
         * @param sentence Input sentence
         * @return Vector of keywords
         */
        std::vector<std::string> extractKeywords(const std::string& sentence)const;
        
        /**
         * @brief Determine request level based on keywords
         * @param keywords Vector of keywords
         * @return RequestLevel priority
         */
        RequestLevel determineLevel(const std::vector<std::string>& keywords)const;
        
    public:
        /**
         * @brief Constructor with message and customer
         * @param msg Customer request message
         * @param customer Pointer to customer making request
         */
        Request(const std::string& msg, Customer* customer);
        
        /**
         * @brief Default constructor
         */
        Request();
        
        /**
         * @brief Destructor
         */
        ~Request();
        
        /**
         * @brief Parse English sentence and determine request level
         * @param sentence Customer request in English
         */
        void parseRequest(const std::string& sentence);
        
        /**
         * @brief Get the request message
         * @return Request message string
         */
        std::string getMessage()const;
        
        /**
         * @brief Get the request level
         * @return RequestLevel enum value
         */
        RequestLevel getLevel()const;
        
        /**
         * @brief Check if request has been handled
         * @return True if handled, false otherwise
         */
        bool isHandled()const;
        
        /**
         * @brief Mark request as handled
         */
        void markHandled();
        
        /**
         * @brief Set request level manually
         * @param lvl RequestLevel to set
         */
        void setLevel(RequestLevel lvl);
        
        /**
         * @brief Convert request to string representation
         * @return String description of request
         */
        std::string toString()const;
        
        /**
         * @brief Extract potential plant name from request
         * @return Plant name if found, empty string otherwise
         */
        std::string extractPlantName()const;
        
        /**
         * @brief Get the customer who made this request
         * @return Pointer to requesting customer
         */
        Customer* getCustomer() const;
};

#endif