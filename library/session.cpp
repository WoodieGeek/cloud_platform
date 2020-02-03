#include "session.h"

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
            std::cout << "!!\n";
            std::cout << std::string(Data_, length) << std::endl;
            DoWrite(length);
        }
    });
}

void TSession::DoWrite(std::size_t) {
    auto self(shared_from_this());
    std::string msg = "HTTP/1.1 200 OK\r\n\r\nSOME TEXT";
    boost::asio::async_write(Socket_, boost::asio::buffer(msg), [this, self] (boost::system::error_code ec, std::size_t) {
        if (!ec) {
            Socket_.close();
        }
    });
}
