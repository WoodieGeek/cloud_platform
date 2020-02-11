#include "session.h"
#include "resolver.h"
#include "reply.h"
#include "request_parser.h"

#include <string>
#include <iostream>

TSession::TSession(boost::asio::ip::tcp::socket socket)
    : Socket_(std::move(socket))
{}

void TSession::Start() {
    DoRead();
}

void TSession::DoRead() {
    auto self(shared_from_this());
    Socket_.async_read_some(boost::asio::buffer(Data_, MESSAGE_LENGTH), [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            TResolver resolver;
            const auto& request = TRequestParser::Parse(std::string(Data_, length));
            TReply reply;
            if (request) {
                reply.Content = resolver.Resolve(*request);
                reply.Status = TReply::StatusType::OK;
            }
            else {
                reply.Content = "Bad request";
                reply.Status = TReply::StatusType::NOT_FOUND;
            }
            DoWrite(reply.Serialize());
        }
    });
}

void TSession::DoWrite(std::string data) {
    auto self(shared_from_this());
    boost::asio::async_write(Socket_, boost::asio::buffer(data), [this, self] (boost::system::error_code ec, std::size_t) {
        if (!ec) {
            Socket_.close();
        }
    });
}
