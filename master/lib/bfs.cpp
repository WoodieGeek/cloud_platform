#include "bfs.h"

TBfs::TBfs(pqxx::connection& connection, TInstancesHolder& instancesHolder, TGraph graph, std::string input)
    : Connection_(connection)
    , InstancesHolder_(instancesHolder)
    , Graph_(graph)
    , Input_(input)
{

    PrepareTasks();
    PrepareNodes();
}

std::unordered_map<std::string, std::string> TBfs::GetBinaries() {
    std::unordered_map<std::string, std::string> result;
    std::string tableName = "jaunt.binaries";
    pqxx::work worker{Connection_};
    std::stringstream query;
    query << "SELECT name, binary_base64 FROM " << tableName << " WHERE graph_id = " << Graph_.GraphID;
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

void TBfs::PrepareTasks() {
    for (const auto& [name, binary] : GetBinaries()) {
        TTask task;
        task.Binary = binary;
        Tasks_[name] = std::move(task);
    }
    Results_[START_NODE] = Input_;
    TFile file{START_NODE, Input_};
    for (const auto& value : Graph_.Graph[START_NODE]) {
        Tasks_[value].Inputs.emplace_back(file);
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
}

void TBfs::Go(const std::string& node) {
    for (const auto& to : Graph_.Graph[node]) {
        Tasks_[to].Inputs.push_back({node, Results_[node]});
        InputNodes_[to]--;
        if (InputNodes_[to] == 0) {
            if (to == FINISH_NODE)  {
            } else {
                InstancesHolder_.Run(Tasks_[to], [this, node = to] (const TTaskResult& result) {
                    Results_[node] = result.Result;
                    Go(node);
                });
            }
        }
    }
}
