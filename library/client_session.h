#pragma once
#include "request.h"
#include "reply.h"
#include <string>
#include <boost/asio.hpp>

class TClientSession : public std::enable_shared_from_this<TClientSession> {
public:
    TClientSession(boost::asio::ip::tcp::socket socket,
                   boost::asio::ip::tcp::endpoint endpoint,
                   TRequest request,
                   std::function<void(const TReply&)> callback);
    void Start();
private:
    void Connect();
    void DoWrite(std::string data);
    void DoRead();
private:
    boost::asio::ip::tcp::socket Socket_;
    boost::asio::ip::tcp::endpoint Endpoint_;
    TRequest Request_;
    std::function<void(const TReply&)> Callback_;
    static constexpr std::size_t MESSAGE_LENGTH = 102400;
    char Data_[MESSAGE_LENGTH];
};
