#include "../../library/server.h"
#include "subprocess.h"

class TServerWrapper {
public:
    TServerWrapper(const std::string& address, const std::string& port);
private:
    TReply Run(const TRequest& request);
    TReply Stop(const TRequest&);
    TReply IsRunning(const TRequest&);
    TReply GetResponse(const TRequest&);
private:
    TServer Server_;
    std::unique_ptr<TSubProcess> Process_;
};
