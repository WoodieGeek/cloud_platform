#include "server.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

TMasterServer::TMasterServer(const std::string& address,
                             const std::string& port,
                             const std::string& connection)
    : Server_(address, port)
    , Connection_(connection)
{
    Server_.AddHandler("/create", [this](const TRequest& request) {
            return Create(request);
    });
    Server_.Run();
}

TReply TMasterServer::Create(const TRequest& request) {
    std::string_view graphName;
    for (const auto& [key, value] : request.Cgi) {
        if (key == "name") {
            graphName = value;
        }
    }
    if (!graphName.empty()) {
        pqxx::work W{Connection_};
        std::stringstream query;
        query << "INSERT INTO  jaunt.graphs_name (name) VALUES ('" << graphName << "') RETURNING id";
        pqxx::result resultInsert{W.exec(query.str())};
        W.commit();
        if (!resultInsert.empty() && !resultInsert.front().empty()) {
            return {std::string{resultInsert.front().front().view()}, TReply::EStatusType::OK};
        }
    }
    return {"Bad request", TReply::EStatusType::NOT_FOUND};
}

TReply TMasterServer::Create(const TRequest& request) {
    return {"Bad request", TReply::EStatusType::NOT_FOUND};
}


