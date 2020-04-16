#include "client_session.h"
#include "reply_parser.h"
#include <iostream>

using namespace boost::asio;

TClientSession::TClientSession(ip::tcp::socket socket,
                               ip::tcp::endpoint endpoint,
                               TRequest request,
                               std::function<void(const TReply&)> callback) 
    : Socket_(std::move(socket))
    , Endpoint_(endpoint)
    , Request_(request)
    , Callback_(callback)
{
}

void TClientSession::Start() {
    Connect();
}

void TClientSession::Connect() {
    auto self(shared_from_this());
    Socket_.async_connect(Endpoint_, [this, self] (const boost::system::error_code& error) {
        if (!error) {
            DoWrite(Request_.Serialize());
        }
    });
}

void TClientSession::DoWrite(std::string data) {
    auto self(shared_from_this());
    boost::asio::async_write(Socket_, boost::asio::buffer(data), [this, self] (const boost::system::error_code& error, std::size_t) {
        if (!error) {
            DoRead();
        }
    });
}

void TClientSession::DoRead() {
    auto self(shared_from_this());
    Socket_.async_read_some(boost::asio::buffer(Data_, MESSAGE_LENGTH), [this, self] (const boost::system::error_code& error, std::size_t length) {
        std::cerr << error.message();
        if (!error) {
            auto reply = TReplyParser::Parse(std::string{Data_, length});
            if (reply) {
                Callback_(*reply);
            }
        }
    });
}

