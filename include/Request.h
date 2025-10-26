// include/Request.h
#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>

/**
 * @enum RequestLevel
 * @brief Priority levels for requests in chain of responsibility
 */
enum class RequestLevel{
    LOW,      // Handled by SalesAssistant
    MEDIUM,   // Handled by FloorManager
    HIGH      // Handled by NurseryOwner
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
        
        /**
         * @brief Extract keywords from sentence
         * @param sentence Input sentence
         * @return Vector of keywords
         */
        std::vector<std::string> extractKeywords(const std::string& sentence);
        
        /**
         * @brief Determine request level based on keywords
         * @param keywords Vector of keywords
         * @return RequestLevel priority
         */
        RequestLevel determineLevel(const std::vector<std::string>& keywords);
        
    public:
        /**
         * @brief Constructor with message
         * @param msg Customer request message
         */
        Request(const std::string& msg);
        
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
};

#endif