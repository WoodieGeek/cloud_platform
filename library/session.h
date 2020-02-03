#pragma once
#include <boost/asio.hpp>

class TSession : public std::enable_shared_from_this<TSession> {
public:
    TSession(boost::asio::ip::tcp::socket socket);
    void Start();
private:
    void DoRead();
    void DoWrite(std::size_t);
private:
    boost::asio::ip::tcp::socket Socket_;
    static constexpr std::size_t MESSAGE_LENGTH = 1024;
    char Data_[MESSAGE_LENGTH];
};
