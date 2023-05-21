/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include <asio.hpp>
#include <iostream>
#include <chrono>
#include <deque>
#include <string_view>
#include "CommunicationManager.h"
#include "../utilities/Logger.h"

class TcpClient
{
public:
    TcpClient(asio::io_context &io_context, const std::string &host, unsigned short port, CommunicationManager &communicationManager)
        : mIoContext(io_context), mStrand(io_context.get_executor()), mResolver(io_context), mHost(host), mPort(port), mCommunicationManager(communicationManager), mSocket(io_context)
    {
        do_connect();
    }

    void write(const std::string &message)
    {
        if (isConnected())
        {
            asio::post(mStrand, [this, message]()
                       {
                                 mResponseQueue.push_back(message);
                                 do_write(); });
        }
        else
        {
            LOG_ERROR("Not connected");
        }
    }

    bool isConnected() const
    {
        return mSocket.is_open();
    }

private:
    void do_connect()
    {
        auto endpoints = mResolver.resolve(mHost, std::to_string(mPort));
        asio::async_connect(
            mSocket, endpoints,
            [this](const asio::error_code &error, const asio::ip::tcp::endpoint & /*endpoint*/)
            {
                if (!error)
                {
                    do_read();
                }
                else
                {
                    mSocket.close();
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    do_connect();
                }
            });
    }

    void do_write()
    {
        if (!mResponseQueue.empty())
        {
            asio::async_write(
                mSocket, asio::buffer(mResponseQueue.front()),
                asio::bind_executor(mStrand, [this](const asio::error_code &error, std::size_t /*length*/)
                                    {
                                    if (!error)
                                    {
                                        mResponseQueue.pop_front();
                                        do_write();
                                    }
                                    else
                                    {
                                        LOG_ERROR("Connection error: %s", error.message().c_str());
                                        mSocket.close();
                                        std::this_thread::sleep_for(std::chrono::seconds(5));
                                        do_connect();
                                    } }));
        }
    }

    void do_read()
    {
        asio::async_read_until(
            mSocket, mStreambuf, '\n',
            asio::bind_executor(mStrand, [this](const asio::error_code &error, std::size_t length)
                                {
                                    if (!error)
                                    {
                                        handleRead(error, length);
                                        do_read();
                                    }
                                    else
                                    {
                                        LOG_ERROR("Read error: %s", error.message().c_str());
                                        mSocket.close();
                                        std::this_thread::sleep_for(std::chrono::seconds(5));
                                        do_connect();
                                    } }));
    }

    void handleRead(const asio::error_code &error, size_t bytes_transferred)
    {
        if (!error)
        {
            std::istream is(&mStreambuf);
            std::string message(std::istreambuf_iterator<char>(is), {});
            mStreambuf.consume(bytes_transferred);
            std::string_view message_view(message.data(), bytes_transferred);
            std::string response = mCommunicationManager.processMessage(message_view);
            // İşlenen mesajı kullanarak ilgili işlemleri gerçekleştirin
        }
    }

    asio::io_context &mIoContext;
    asio::strand<asio::io_context::executor_type> mStrand;
    asio::ip::tcp::resolver mResolver;
    const std::string mHost;
    const unsigned short mPort;
    std::deque<std::string> mResponseQueue;
    CommunicationManager &mCommunicationManager;
    asio::ip::tcp::socket mSocket;
    asio::streambuf mStreambuf;
};