#include "server.h"
#include "bfs.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

TMasterServer::TMasterServer(const std::string& address,
                             const std::string& port,
                             const std::string& connection,
                             TServerOptions serverOptions)
    : ServerOptions_(std::move(serverOptions))
    , Server_(IoSerivce_, address, port)
    , Connection_(connection)
    , InstancesHolder_(IoSerivce_, ServerOptions_.Instances)
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
    Server_.AddHandler("/binary_update", [this](const TRequest& request) {
            return BinaryUpdate(request);
    });
    Server_.AddHandler("/run", [this](const TRequest& request) {
            return RunGraph(request);
    });
    Server_.AddHandler("/graphs", [this](const TRequest& request) {
            return AllGraphs(request);
    });



    Server_.Run();
}

TReply TMasterServer::Create(const TRequest& request) {
    const std::string cgiName = "name";
    const std::string tableName = "jaunt.graphs";
    if (!request.Cgi.count(cgiName)) {
        return {"No name in cgi", TReply::EStatusType::NOT_FOUND};
    }
    std::stringstream query;
    query << "INSERT INTO " << tableName << " (name) VALUES ('" << request.Cgi.find(cgiName)->second << "') RETURNING id";

    try {
        pqxx::work W{Connection_};
        pqxx::result resultInsert{W.exec(query.str())};
        W.commit();
        if (!resultInsert.empty() && !resultInsert.front().empty()) {
            return {std::string{resultInsert.front().front().view()}, TReply::EStatusType::OK};
        }
    } catch (const pqxx::sql_error& error) {
        std::cerr << error.what() << std::endl;
        return {"Bad request", TReply::EStatusType::NOT_FOUND};
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
    std::stringstream query;
    query << "UPDATE " << tableName << " SET graph = '" << request.Content << "' WHERE id = " <<  request.Cgi.find(cgiName)->second;
    try {
        pqxx::work W{Connection_};
        pqxx::result resultSelect{W.exec(query.str())};
        W.commit();
    } catch (const pqxx::sql_error& error) {
        std::cerr << error.what() << std::endl;
        return {"Bad request", TReply::EStatusType::NOT_FOUND};
    }
    return {"DONE", TReply::EStatusType::NOT_FOUND};
}

TReply TMasterServer::RunGraph(const TRequest& request) {
    const std::string cgiName = "id";
    const std::string tableName = "jaunt.results";
    if (!request.Cgi.count(cgiName)) {
        return {"NO ID", TReply::EStatusType::NOT_FOUND};
    }
    std::stringstream query;
    query << "INSERT INTO  " <<  tableName << " (graph_id, start_time, input, status) ";
    query << "VALUES (" << request.Cgi.find(cgiName)->second << ", now(), '" << request.Content << "', 'RUNNING') ";
    query << "RETURNING id ";

    try {
        pqxx::work W{Connection_};
        pqxx::result resultInsert{W.exec(query.str())};
        W.commit();
        std::string resultID;
        if (!resultInsert.empty() && !resultInsert.front().empty()) {
            resultID = resultInsert.front().front().view();
            std::make_shared<TBfs>(Connection_, InstancesHolder_, stoi(resultID))->Start();
            return {"DONE", TReply::EStatusType::OK};
        }
    } catch (const pqxx::sql_error& error) {
        std::cerr << error.what() << std::endl;
    }
    return {"Bad request", TReply::EStatusType::NOT_FOUND};
}

TReply TMasterServer::BinaryUpdate(const TRequest& request) {
    const std::string cgiGraphId = "graph_id";
    const std::string cgiNode = "node";
    const std::string tableName = "jaunt.binaries";
    if (!request.Cgi.count(cgiGraphId) || !request.Cgi.count(cgiNode)) {
        return {"NO graph_id or node", TReply::EStatusType::NOT_FOUND};
    }
    std::stringstream query;
    query << "INSERT INTO " << tableName << "  (graph_id , node , binary_base64) ";
    query << " VALUES (" << request.Cgi.find(cgiGraphId)->second << ", ";
    query << "'" << request.Cgi.find(cgiNode)->second << "', '" << request.Content << "') "; 
    query << "ON CONFLICT (graph_id, node) do update  SET binary_base64 = '" << request.Content << "'";
    try {
        pqxx::work W{Connection_};
        pqxx::result resultUpsert{W.exec(query.str())};
        W.commit();
    } catch (pqxx::sql_error e) {
        std::cerr << e.what() << "sad\n";
        return {"Bad request", TReply::EStatusType::NOT_FOUND};
    }
    return {"DONE", TReply::EStatusType::OK};
}

TReply TMasterServer::AllGraphs(const TRequest&) {
    std::string query = "SELECT id, name FROM jaunt.graphs";
    try {
        pqxx::work W{Connection_};
        pqxx::result resultSelect{W.exec(query)};
        nlohmann::json allGraphs;
        for (const auto& row : resultSelect) {
            if (row.size() != 2) {
                continue;
            }
            nlohmann::json graphJson;
            graphJson["id"] = std::string{row[0].view()};
            graphJson["name"] = std::string{row[1].view()};
            allGraphs.push_back(std::move(graphJson));
        }
        nlohmann::json result;
        result["set"] = std::move(allGraphs);
        return {result.dump(), TReply::EStatusType::OK};
    } catch (const pqxx::sql_error& error) {
        std::cerr << error.what() << std::endl;
    }
    return {{}, TReply::EStatusType::OK};
}
