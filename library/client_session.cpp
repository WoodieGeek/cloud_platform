#include "client_session.h"
#include "reply_parser.h"

using namespace boost::asio;

TClientSession::TClientSession(ip::tcp::socket socket,
                               ip::tcp::endpoint endpoint,
                               TRequest request,
                               std::function<void(const TReply&)> callback) 
    : Socket_(std::move(socket))
    , Callback_(callback)
{
    auto self(shared_from_this());
    Socket_.async_connect(endpoint, [this, self, request] (const boost::system::error_code& error) {
        if (!error) {
            DoWrite(request.Serialize());
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
    boost::asio::async_read(Socket_, boost::asio::buffer(Data_, MESSAGE_LENGTH), [this, self] (const boost::system::error_code& error, std::size_t length) {
        if (!error) {
            auto reply = TReplyParser::Parse(std::string{Data_, length});
            if (reply) {
                Callback_(*reply);
            }
        }
    });
}

