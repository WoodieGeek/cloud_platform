#include "bfs.h"
#include <iostream>

TBfs::TBfs(pqxx::connection& connection, TInstancesHolder& instancesHolder, const int resultID)
    : Connection_(connection)
    , InstancesHolder_(instancesHolder)
    , ResultID_(resultID) 
{
    PrepareGraph();
    PrepareTasks();
    PrepareNodes();
}

std::unordered_map<std::string, std::string> TBfs::GetBinaries() {
    std::unordered_map<std::string, std::string> result;
    std::string tableName = "jaunt.binaries";
    pqxx::work worker{Connection_};
    std::stringstream query;
    query << "SELECT node, binary_base64 FROM " << tableName << " WHERE graph_id = " << GraphID_;
    pqxx::result resultSelect{worker.exec(query.str())};
    worker.commit();
    for (const auto& row : resultSelect) {
        if (row.size() != 2) {
            continue;
        }
        result[row[0].c_str()] = row[1].c_str();
    }
    return result;
}

void TBfs::PrepareGraph() {
    const std::string jauntResult = "jaunt.results";
    const std::string jauntGraph = "jaunt.graphs";
    pqxx::work worker{Connection_};
    std::stringstream query;
    query << "SELECT graph.graph, result.input, graph.id FROM " << jauntResult << " as result ";
    query << " JOIN " << jauntGraph << " as graph ";
    query << " ON result.graph_id = graph.id ";
    query << " WHERE result.id = " << ResultID_;
    pqxx::result resultSelect{worker.exec(query.str())};
    worker.commit();
    if (!resultSelect.empty() && resultSelect.front().size() == 3) {
        const auto& row = resultSelect.front();
        Graph_ = TGraph{row[0].c_str()};
        Input_ = row[1].c_str();
        GraphID_ = stoi(std::string{row[2].c_str()});
    }
}

void TBfs::PrepareTasks() {
    for (const auto& [name, binary] : GetBinaries()) {
        TTask task;
        task.Binary = binary;
        Tasks_[name] = std::move(task);
    }
}

void TBfs::PrepareNodes() {
    for (const auto& [_, nodes] : Graph_.Graph) {
        for (const auto& node : nodes) {
            InputNodes_[node]++;
        }
    }
}

void TBfs::Start() {
    Go(START_NODE);
}

void TBfs::Go(const std::string& node) {
    for (const auto& to : Graph_.Graph[node]) {
        Tasks_[to].Inputs.push_back({node, Results_[node]});
        InputNodes_[to]--;
        if (InputNodes_[to] == 0) {
            if (to == FINISH_NODE)  {
                Results_[to] = Results_[node];
                SetGraphResult();
            } else {
                auto self(shared_from_this());
                InstancesHolder_.Run(Tasks_[to], [this, self, node = to] (const TTaskResult& result) {
                    Results_[node] = result.Result;
                    Go(node);
                });
            }
        }
    }
}

void TBfs::SetNodeResult(const std::string& node, const std::string& result) {
    const std::string& tableName = "jaunt.node_result";
    pqxx::work worker{Connection_};
    std::stringstream query;
    query << "UPDATE " << tableName << " SET finish_time = now(), result = " << result;
    query << " WHERE result_id = " << ResultID_ <<  " AND name = " << node ;
    pqxx::result resultUpdate{worker.exec(query.str())};
    worker.commit();
}

void TBfs::SetGraphResult() {
    const std::string& tableName = "jaunt.results";
    std::stringstream query;
    query << "UPDATE " << tableName << " SET status = 'FINISHED', finish_time = now(), output = '" << Results_[FINISH_NODE] << "' ";
    query << " WHERE id = " << ResultID_;
    try {
        pqxx::work worker{Connection_};
        pqxx::result resultUpdate{worker.exec(query.str())};
        worker.commit();
    } catch (const pqxx::sql_error& error) {
        std::cerr << error.what() << std::endl;
    }
}

void TBfs::SetNodeStartTime(const std::string& node) {
    const std::string& tableName = "jaunt.node_result";
    pqxx::work worker{Connection_};
    std::stringstream query;
    query << "UPDATE " << tableName << " SET start_time = now() " << Results_[FINISH_NODE];
    query << " WHERE result_id = " << ResultID_ << " AND name = " << node;
    pqxx::result resultUpdate{worker.exec(query.str())};
    worker.commit();
}

