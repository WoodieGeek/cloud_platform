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
    Server_.AddHandler("/results", [this](const TRequest& request) {
            return Results(request);
    });
    Server_.AddHandler("/graph", [this](const TRequest& request) {
            return Graph(request);
    });
    Server_.AddHandler("/graph_update", [this](const TRequest& request) {
            return GraphUpdate(request);
    });
    Server_.Run();
}

TReply TMasterServer::Create(const TRequest& request) {
    const std::string cgiName = "name";
    const std::string tableName = "jaunt.graphs";
    if (!request.Cgi.count(cgiName)) {
        return {"No name in cgi", TReply::EStatusType::NOT_FOUND};
    }
    pqxx::work W{Connection_};
    std::stringstream query;
    query << "INSERT INTO " << tableName << " (name) VALUES ('" << request.Cgi.find(cgiName)->second << "') RETURNING id";
    pqxx::result resultInsert{W.exec(query.str())};
    W.commit();
    if (!resultInsert.empty() && !resultInsert.front().empty()) {
        return {std::string{resultInsert.front().front().view()}, TReply::EStatusType::OK};
    }
    return {"Bad request", TReply::EStatusType::NOT_FOUND};
}

TReply TMasterServer::Results(const TRequest& request) {
    const std::string cgiName = "id";
    const std::string tableName = "jaunt.results";
    if (!request.Cgi.count(cgiName)) {
        return {"No ID", TReply::EStatusType::NOT_FOUND};
    }
    pqxx::work W{Connection_};
    std::stringstream query;
    query << "SELECT start_time, finish_time, input, output, status FROM " << tableName;
    query << " WHERE graph_id = " << request.Cgi.find(cgiName)->second;
    pqxx::result resultSelect{W.exec(query.str())};
    W.commit();
    nlohmann::json resultJson;
    for (const auto& row : resultSelect) {
        nlohmann::json rowJson;
        for (const auto& field : row) {
            rowJson[field.name()] = field.c_str();  
        }
        resultJson.push_back(std::move(rowJson));
    }
    return {resultJson.dump(), TReply::EStatusType::NOT_FOUND};
}

TReply TMasterServer::Graph(const TRequest& request) {
    const std::string cgiName = "id";
    const std::string tableName = "jaunt.graphs";
    if (!request.Cgi.count(cgiName)) {
        return {"No ID", TReply::EStatusType::NOT_FOUND};
    }
    pqxx::work W{Connection_};
    std::stringstream query;
    query << "SELECT graph FROM " << tableName << " WHERE id = " <<  request.Cgi.find(cgiName)->second;
    pqxx::result resultSelect{W.exec(query.str())};
    W.commit();
    if (!resultSelect.empty() && !resultSelect.front().empty()) {
        return {std::string{resultSelect.front().front().view()}, TReply::EStatusType::OK};
    }
    return {"Bad request", TReply::EStatusType::NOT_FOUND};
}

TReply TMasterServer::GraphUpdate(const TRequest& request) {
    const std::string cgiName = "id";
    const std::string tableName = "jaunt.graphs";
    if (!request.Cgi.count(cgiName)) {
        return {"No ID", TReply::EStatusType::NOT_FOUND};
    }
    pqxx::work W{Connection_};
    std::stringstream query;
    query << "UPDATE " << tableName << " SET graph = '" << request.Content << "' WHERE id = " <<  request.Cgi.find(cgiName)->second;
    pqxx::result resultSelect{W.exec(query.str())};
    W.commit();
    return {"DONE", TReply::EStatusType::NOT_FOUND};
}

