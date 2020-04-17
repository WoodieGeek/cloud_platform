#include "client.h"
#include "client_session.h"
#include "reply_parser.h"
#include <iostream>

using namespace boost::asio;


TClient::TClient(boost::asio::io_service& ioService)
    : IoService_(ioService) {
};

void TClient::HttpRequest(const std::string& address,
                          const int port,
                          const TRequest& request,
                          std::function<void(const TReply&)> callback)
{
    ip::tcp::endpoint endpoint(ip::address::from_string(address), port);
    ip::tcp::socket socket(IoService_);
    std::make_shared<TClientSession>(std::move(socket), endpoint, request, callback)->Start();
};

TReply TClient::HttpRequest(const std::string& address,
                            const int port,
                            const TRequest& request)
{
    ip::tcp::endpoint endpoint(ip::address::from_string(address), port);
    ip::tcp::socket socket(IoService_);
    socket.connect(endpoint);
    boost::system::error_code error;
    socket.write_some(boost::asio::buffer(request.Serialize()),  error);
    TReply errorReply;
    errorReply.Status = TReply::EStatusType::NOT_FOUND;
    if (error) {
        std::cerr << error.message() << std::endl;
        return errorReply;
    }
    char data[10000];
    auto length = socket.read_some(boost::asio::buffer(data));
    auto reply = TReplyParser::Parse(std::string{data, length});
    if (reply) {
        return *reply;
    }
    return errorReply;
};

