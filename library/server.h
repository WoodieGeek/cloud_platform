#pragma once
#include "resolver.h"
#include <boost/asio.hpp>

class TServer {
public:
    TServer(const TServer&) = delete;
    TServer& operator = (const TServer&) = delete;

    explicit TServer(const std::string& address, const std::string& port);
    void Run();
    void AddHandler(const std::string& handleName, const std::function<std::string(TRequest)>& handler);
private:
    void DoAccept();
private:
    TResolver Resolver_;
    boost::asio::io_service IoService_;
    boost::asio::ip::tcp::acceptor Acceptor_;
    boost::asio::ip::tcp::socket Socket_;
};
