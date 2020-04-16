#include "client.h"
#include "client_session.h"

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

