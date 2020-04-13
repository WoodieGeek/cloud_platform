#pragma once 
#include "instances.h"
#include "graph.h"

#include <pqxx/pqxx>

class TBfs {
public:
    TBfs(pqxx::connection& connection,
         TInstancesHolder& instancesHolder,
         TGraph graph,
         std::string input);
    void Start();
private:
    std::unordered_map<std::string, std::string> GetBinaries();
    void PrepareTasks();
    void PrepareNodes();
    void Go(const std::string& node);
private:
    pqxx::connection& Connection_;
    TInstancesHolder& InstancesHolder_;
    TGraph Graph_;
    std::string Input_;

    std::unordered_map<std::string, TTask> Tasks_;                                                                                                                                                             
    std::unordered_map<std::string, std::string> Results_;
    std::unordered_map<std::string, int> InputNodes_;

    const std::string START_NODE = "REQUEST";
    const std::string FINISH_NODE = "RESPONSE";
};
