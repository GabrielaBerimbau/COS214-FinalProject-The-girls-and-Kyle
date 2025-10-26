#include "../include/Request.h"
#include <algorithm>
#include <sstream>
#include <iostream>

Request::Request(const std::string& msg): message(msg), level(RequestLevel::LOW), handled(false){
    parseRequest(msg);
}

Request::Request(): message(""), level(RequestLevel::LOW), handled(false){}

Request::~Request(){}

std::vector<std::string> Request::extractKeywords(const std::string& sentence)const{
    std::vector<std::string> keywords;
    std::stringstream ss(sentence);
    std::string word;
    
    while(ss >> word){
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        
        if(!word.empty()){
            keywords.push_back(word);
        }
    }
    
    return keywords;
}

RequestLevel Request::determineLevel(const std::vector<std::string>& keywords)const{
    for(const std::string& keyword: keywords){
        if(keyword == "complaint" || keyword == "refund" || 
           keyword == "manager" || keyword == "urgent" || 
           keyword == "emergency" || keyword == "lawsuit" ||
           keyword == "sue" || keyword == "legal"){
            return RequestLevel::HIGH;
        }
    }
    
    for(const std::string& keyword: keywords){
        if(keyword == "bulk" || keyword == "wedding" || 
           keyword == "special" || keyword == "order" || 
           keyword == "custom" || keyword == "arrangement" || 
           keyword == "corporate" || keyword == "large" ||
           keyword == "100" || keyword == "50" || keyword == "dozen" ||
           keyword == "event" || keyword == "party"){
            return RequestLevel::MEDIUM;
        }
    }
    
    for(const std::string& keyword: keywords){
        if(keyword == "cactus" || keyword == "aloe" || keyword == "succulent"){
            return RequestLevel::LOW;
        }

        if(keyword == "potato" || keyword == "radish" || keyword == "vegetable"){
            return RequestLevel::LOW;
        }
        
        if(keyword == "rose" || keyword == "daisy" || keyword == "flower" ||
           keyword == "strelitzia"){
            return RequestLevel::LOW;
        }
        
        if(keyword == "venus" || keyword == "flytrap" || keyword == "venusflytrap" ||
           keyword == "monstera"){
            return RequestLevel::LOW;
        }
        
        if(keyword == "plant" || keyword == "plants"){
            return RequestLevel::LOW;
        }
    }
    
    
    return RequestLevel::LOW;
}

void Request::parseRequest(const std::string& sentence){
    message = sentence;
    std::vector<std::string> keywords = extractKeywords(sentence);
    level = determineLevel(keywords);
}

std::string Request::getMessage()const{
    return message;
}

RequestLevel Request::getLevel()const{
    return level;
}

bool Request::isHandled()const{
    return handled;
}

void Request::markHandled(){
    handled = true;
}

void Request::setLevel(RequestLevel lvl){
    level = lvl;
}

std::string Request::toString()const{
    std::string levelStr;
    
    if(level == RequestLevel::LOW){
        levelStr = "LOW";
    } 

    else if(level == RequestLevel::MEDIUM){
        levelStr = "MEDIUM";
    } 

    else{
        levelStr = "HIGH";
    }
    
    std::string handledStr = handled ? "Yes" : "No";
    
    return "Request: '" + message + "' [Level: " + levelStr + ", Handled: " + handledStr + "]";
}

std::string Request::extractPlantName()const{
    std::vector<std::string> keywords = extractKeywords(message);
    
    std::vector<std::string> plantTypes = {
        "cactus", "aloe", "succulent",
        "potato", "radish", "vegetable",
        "rose", "daisy", "flower", "strelitzia",
        "venusflytrap", "venus", "flytrap", "monstera",
        "plant"
    };
    
    for(const std::string& keyword: keywords){
        for(const std::string& plantType: plantTypes){
            
            if(keyword == plantType){
                return plantType;
            }
        }
    }
    
    return "";
}