# Asio Network Library

This library provides a simple and easy-to-use interface for TCP networking using Asio, a popular C++ networking library. It includes a `TcpServer`, `TcpClient`, and a `CommunicationManager` for handling network communication between clients and servers.

## Features

- Asynchronous TCP server and client implementation using Asio
- IPv6 support
- Message processing through `CommunicationManager` with a callback system
- Configuration file support for the `CommunicationManager`

## Dependencies

- [Asio](https://think-async.com/Asio/) - Asio C++ Library
- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++

## Installation

1. Clone the repository: `git clone git@github.com:ahmetihsankose/network_communication.git`
2. `git submodule update --init --recursive`

## Usage

1. Include the necessary header files in your project.
2. Create a `CommunicationManager` object and register handlers for different message types using `registerHandler`.
3. Instantiate the `TcpServer` or `TcpClient` with the desired configuration (IP address, port, etc.).
4. Start the server or client as needed.

See the provided example code for a more detailed demonstration of how to use the library.

## Example

The following example demonstrates how to create a simple echo server and client using the library:

Create a config.json:
    
```json
{
    "server_ip": "127.0.0.1",
    "server_port": 9000,
    "client_ip": "127.0.0.1",
    "client_port": 9000
}
```

Then create a server.cpp and client.cpp:

```cpp // server.cpp
#include "TcpServer.h"
#include "CommunicationManager.h"
#include <iostream>

int main()
{
    try
    {
        asio::io_context io_context;
        CommunicationManager communicationManager("config.json");

        communicationManager.registerHandler("greet", [](const nlohmann::json &message) {
            std::string name = message["name"];
            return "Hello, " + name + "!";
        });

        TcpServer server(io_context, communicationManager.getServerPort(), communicationManager);
        io_context.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

```cpp // client.cpp
#include "TcpClient.h"
#include "CommunicationManager.h"
#include <iostream>
#include <thread>

int main()
{
    try
    {
        asio::io_context io_context;
        CommunicationManager communicationManager("config.json");

        TcpClient client(io_context, communicationManager.getClientIP(), communicationManager.getServerPort(), communicationManager);

        std::thread t([&io_context]() { io_context.run(); });

        // Wait for the client to connect to the server
        while (!client.isConnected())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Send a greet message from the client
        nlohmann::json greet_message = {{"message_type", "greet"}, {"name", "Ihsan"}};
        client.write(greet_message.dump());

        t.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```
