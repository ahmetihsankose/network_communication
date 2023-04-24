/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include "TcpConnection.h"
#include "CommunicationManager.h"

class TcpServer
{
public:
    TcpServer(asio::io_context &io_context, unsigned short port, CommunicationManager &communicationManager)
        : mAcceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v6(), port)), mCommunicationManager(communicationManager)
    {
        do_accept();
    }
    
private:
    void do_accept()
    {
        mAcceptor.async_accept(
            [this](const asio::error_code &error, asio::ip::tcp::socket socket)
            {
                if (!error)
                {
                    std::cout << "New connection from " << socket.remote_endpoint().address().to_string() << std::endl;
                    std::make_shared<TcpConnection>(std::move(socket), mCommunicationManager)->start();
                }

                do_accept();
            });
    }

    asio::ip::tcp::acceptor mAcceptor;
    CommunicationManager &mCommunicationManager;
};