#include <iostream>
#include <asio.hpp>
#include "TcpClient.h"
#include "CommunicationManager.h"

int main(int argc, char **argv)
{
    asio::io_context io_context;
    CommunicationManager communicationManager;

    std::string host = "127.0.0.1";
    unsigned short port = 8081;

    TcpClient client(io_context, host, port, communicationManager);
    
    std::thread io_thread([&io_context]()
                          { io_context.run(); });

    // nlohmann::json json_message;
    // json_message["message_type"] = "COMMAND";
    // json_message["module"] = "motion";
    // json_message["action"] = "start";
    // json_message["data"]["velocity"] = 10;

    // client.write(json_message.dump());

    while (true)
    {
        std::string input;
        std::cout << "Enter a command: ";
        std::getline(std::cin, input);

        if (input == "exit")
        {
            break;
        }

        client.write(input);
    }

    io_thread.join();

    return 0;
}