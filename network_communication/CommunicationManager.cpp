/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(const std::string &config_file_path)
{
    if (!config_file_path.empty())
    {
        loadConfiguration(config_file_path);
    }
}

void CommunicationManager::loadConfiguration(const std::string &config_file_path)
{
    std::ifstream config_file(config_file_path);
    if (config_file.is_open())
    {
        config_file >> mConfigJson;
        config_file.close();
    }
    else
    {
        throw std::runtime_error("Could not open the configuration file.");
    }
}

void CommunicationManager::registerHandler(const std::string &message_type, CallbackFunction callback)
{
    mCallbacks[message_type] = callback;
}

std::string CommunicationManager::processMessage(std::string_view message)
{
    try
    {
        nlohmann::json json_message = nlohmann::json::parse(message);
        std::string message_type = json_message["message_type"];

        if (mCallbacks.find(message_type) != mCallbacks.end())
        {
            return mCallbacks[message_type](json_message);
        }
        else
        {
            return "Invalid message type";
        }
    }
    catch (const nlohmann::json::exception &e)
    {
        return std::string("JSON error: ") + e.what();
    }
}

unsigned short CommunicationManager::getServerPort() const
{
    return mConfigJson.value("server_port", 0);
}

std::string CommunicationManager::getServerIP() const
{
    return mConfigJson.value("server_ip", "127.0.0.1");
}

unsigned short CommunicationManager::getClientPort() const
{
    return mConfigJson.value("client_port", 0);
}

std::string CommunicationManager::getClientIP() const
{
    return mConfigJson.value("client_ip", "127.0.0.1");
}