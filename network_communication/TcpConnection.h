/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include <asio.hpp>
#include <memory>
#include <string>
#include <iostream>
#include "CommunicationManager.h"
#include <queue>

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    using Socket = asio::ip::tcp::socket;
    using Ptr = std::shared_ptr<TcpConnection>;

    TcpConnection(asio::ip::tcp::socket socket, CommunicationManager &communicationManager)
        : mSocket(std::move(socket)), mCommunicationManager(communicationManager)
    {
    }

    static Ptr create(asio::ip::tcp::socket socket, CommunicationManager &communicationManager)
    {
        return Ptr(new TcpConnection(std::move(socket), communicationManager));
    }

    Socket &socket()
    {
        return mSocket;
    }

    void start()
    {
        do_read();
    }

private:
    void handleRead(const asio::error_code &error, size_t bytes_transferred)
    {
        if (!error)
        {
            std::string_view message_view(mData.data(), bytes_transferred);
            std::string response = mCommunicationManager.processMessage(message_view);
            // ...

            mResponseQueue.push(response);
            do_write();
            do_read();
        }
        else if (error != asio::error::operation_aborted)
        {
            std::cout << "Error: " << error.message() << std::endl;
        }
        else if (error == asio::error::eof)
        {
            std::cout << "Client disconnected" << std::endl;
            mSocket.close();
            mResponseQueue = std::queue<std::string>(); // Clear the queue
        }
    }

    void do_read()
    {
        auto self(shared_from_this());
        mSocket.async_read_some(
            asio::buffer(mData),
            [this, self](const asio::error_code &error, std::size_t length)
            {
                if (!error)
                {
                    std::string message(mData.data(), length);
                    std::cout << "Received message: " << message << std::endl;
                    handleRead(error, length);
                    do_read();
                }
                else if (error == asio::error::connection_reset ||
                         error == asio::error::eof ||
                         error == asio::error::timed_out)
                {
                    // Bağlantıyı kapatın ve hatayı yazdırın.
                    std::cerr << "Connection error: " << error.message() << std::endl;
                    mSocket.close();
                }
            });
    }

    void do_write()
    {
        if (!mResponseQueue.empty())
        {
            auto self(shared_from_this());

            asio::async_write(
                mSocket, asio::buffer(mResponseQueue.front()),
                [this, self](const asio::error_code &error, std::size_t /*length*/)
                {
                    if (!error)
                    {
                        mResponseQueue.pop();
                        if (!mResponseQueue.empty())
                        {
                            do_write();
                        }
                    }
                    else
                    {
                        std::cerr << "Connection error: " << error.message() << std::endl;
                        mSocket.close();
                    }
                });
        }
    }

    std::queue<std::string> mResponseQueue;

    Socket mSocket;
    std::array<char, 128> mData;
    CommunicationManager mCommunicationManager;
};
