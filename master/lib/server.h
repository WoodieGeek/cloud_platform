#include "../../library/server.h"
#include "instances.h"

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
    TReply RunGraph(const TRequest& reqeust);
    TReply BinaryUpdate(const TRequest& request);
private:
    boost::asio::io_service IoSerivce_;
    TServer Server_;
    pqxx::connection Connection_;
    TInstancesHolder InstancesHolder_;
};
