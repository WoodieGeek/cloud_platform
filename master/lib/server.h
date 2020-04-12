#include "../../library/server.h"

#include <pqxx/pqxx>

class TMasterServer {
public:
    TMasterServer (const std::string& address,
                   const std::string& port,
                   const std::string& connection);
private:
    TReply Create(const TRequest& request);
    TReply Results(const TRequest& request);
    TReply Graph(const TRequest& request);
    TReply GraphUpdate(const TRequest& request);
private:
    TServer Server_;
    pqxx::connection Connection_;
};
