#pragma once
#include "resolver.h"
#include <boost/asio.hpp>

class TSession : public std::enable_shared_from_this<TSession> {
public:
    TSession(const TResolver& resolver, boost::asio::ip::tcp::socket socket);
    void Start();
private:
    void DoRead();
    void DoWrite(std::string);
private:
    const TResolver& Resolver_;
    boost::asio::ip::tcp::socket Socket_;
    static constexpr std::size_t MESSAGE_LENGTH = 102400;
    char Data_[MESSAGE_LENGTH];
};
