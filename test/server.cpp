#include <iostream>
#include "TcpServer.h"
#include "CommunicationManager.h"

CommunicationManager communicationManager;

int main(int argc, char **argv)
{
    
    asio::io_context io_context;

    // // Register handlers
    // communicationManager.registerHandler("COMMAND", [](const nlohmann::json &json_message)
    //                                      {
    //     std::string module = json_message["module"];
    //     std::string action = json_message["action"];
    //     nlohmann::json data = json_message["data"];

    //     if (module == "motion" && action == "start")
    //     {
    //         int velocity = data["velocity"];
    //         std::cout << "Velocity: " << velocity << std::endl;
    //         return "Motion started";
    //     }
    //     else if (module == "motion" && action == "stop")
    //     {
    //     std::cout << "Motion stop command received" << std::endl;
    //     return "Motion stopped";
    //     }
    //     else
    //     {
    //     return "Invalid command";
    //     } });

    communicationManager.registerHandler("COMMAND", [](const std::string &message)
                                         {
        std::cout << "Received message: " << message << std::endl;
        return "Received message: " + message;
    });

    // Create TcpServer instance and use communication manager
    unsigned short port = 8081;

    TcpServer server(io_context, port, communicationManager);

    // Move I/O context processing to another thread to run asynchronously
    std::thread io_thread([&io_context]()
                          { io_context.run(); });

    // Main thread continues here and can do other tasks if needed

    // When main thread terminates, join the I/O thread to wait for it
    io_thread.join();

    return 0;
}
