#include "server.h"
#include "session.h"

#include <bits/stdc++.h>

TServer::TServer(const std::string& address, const std::string& port)
    : Acceptor_(IoService_)
    , Socket_(IoService_)
{
    std::cout << "construct server\n";
    boost::asio::ip::tcp::resolver resolver(IoService_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
    Acceptor_.open(endpoint.protocol());
    Acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    Acceptor_.bind(endpoint);
    Acceptor_.listen();
    DoAccept();
}

void TServer::DoAccept() {
    Acceptor_.async_accept(Socket_, [this](boost::system::error_code ec) {
            if (!Acceptor_.is_open()) {
                std::cout << "acceptor is not open\n";
                return;
            }
            if (!ec) {
                std::make_shared<TSession>(std::move(Socket_))->Start();
            }
            DoAccept();
    });
}

void TServer::Run() {
    IoService_.run();
}


