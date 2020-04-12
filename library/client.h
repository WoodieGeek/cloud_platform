#pragma once 

#include "reply.h"
#include "request.h"

#include <string>
#include <vector>
#include <boost/asio.hpp>


class TClient {
public:
    TClient(boost::asio::io_service& ioService);
    void HttpRequest(const std::string& address,
                       const int port,
                       const TRequest& request,
                       std::function<void(const TReply&)> callback);
private:
    boost::asio::io_service& IoService_;
};
