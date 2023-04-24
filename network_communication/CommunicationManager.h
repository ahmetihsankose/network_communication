/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include <string>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <nlohmann/json.hpp>

class CommunicationManager
{
public:
    using CallbackFunction = std::function<std::string(const nlohmann::json &)>;

    CommunicationManager(const std::string &config_file_path);
    CommunicationManager() : CommunicationManager("") {}; // Delegating constructor
    
    void registerHandler(const std::string &message_type, CallbackFunction callback);

    std::string processMessage(std::string_view message);

    unsigned short getServerPort() const;
    std::string getServerIP() const;
    unsigned short getClientPort() const;
    std::string getClientIP() const;

private:
    void loadConfiguration(const std::string &config_file_path);

    nlohmann::json mConfigJson;
    std::unordered_map<std::string, CallbackFunction> mCallbacks;
};