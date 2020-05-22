#include "server_options.h"
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

TServerOptions TServerOptions::Parse(const std::string& data) {
    TServerOptions result;
    auto jsonData = nlohmann::json::parse(data);
    const auto& instances = jsonData.at("instances");
    for (const auto& item : instances) {
        unsigned int port = stoi(item.at("port").get<std::string>());
        result.Instances.push_back({item.at("ip"), port}); 
    }
    return result;
}
