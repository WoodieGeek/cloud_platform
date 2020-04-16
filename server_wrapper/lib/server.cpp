#include "server.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "base64.hpp"

TServerWrapper::TServerWrapper(const std::string& address, const std::string& port)
    : Server_(IoService_, address, port) {
    Server_.AddHandler("/run", [this](const TRequest& request) {
            return Run(request);
    });
    Server_.AddHandler("/stop", [this](const TRequest& request) {
            return Stop(request);
    });
    Server_.AddHandler("/is_running", [this](const TRequest& request) {
            return IsRunning(request);
    });
    Server_.AddHandler("/get_response", [this](const TRequest& request) {
            return GetResponse(request);
    });
    Server_.Run();
}

TReply TServerWrapper::Run(const TRequest& request) {
    auto requestJson = nlohmann::json::parse(request.Content);
    std::ofstream outBinary {"binary"};
    outBinary << boost::beast::detail::base64_decode(requestJson["binary"]);
    outBinary.close();
    boost::process::child chmod("chmod +x binary");
    chmod.wait();
    Process_.reset(new TSubProcess("./binary >> out.txt"));
    return {"OK", TReply::EStatusType::OK};
}

TReply TServerWrapper::Stop(const TRequest&) {
    if (Process_) {
        Process_->Stop();
        return {"Done", TReply::EStatusType::OK};
    } else {
        return {"Process does not exist", TReply::EStatusType::OK};
    }
}
TReply TServerWrapper::IsRunning(const TRequest&) {
    if (Process_) {
        return {Process_->IsRunning() ? "true" : "false", TReply::EStatusType::OK};
    } else {
        return {"Process does not exist", TReply::EStatusType::OK};
    }
}
TReply TServerWrapper::GetResponse(const TRequest&) {
    std::ifstream inFile {"out.txt"};
    if (inFile.is_open()) {
        std::stringstream result;
        result << inFile.rdbuf();
        return  {result.str(), TReply::EStatusType::OK};
    } else {
        return {"Output file  does not exist", TReply::EStatusType::OK};
    }
}

