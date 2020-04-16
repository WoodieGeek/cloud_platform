#pragma once 
#include "instances.h"
#include "graph.h"

#include <pqxx/pqxx>

class TBfs : public std::enable_shared_from_this<TBfs> {
public:
    TBfs(pqxx::connection& connection,
         TInstancesHolder& instancesHolder,
         const int resultID);
    void Start();
private:
    std::unordered_map<std::string, std::string> GetBinaries();
    void PrepareGraph();
    void PrepareTasks();
    void PrepareNodes();
    void Go(const std::string& node);
    void SetNodeResult(const std::string& node, const std::string& result);
    void SetGraphResult();
    void SetNodeStartTime(const std::string& node);
private:
    pqxx::connection& Connection_;
    TInstancesHolder& InstancesHolder_;
    TGraph Graph_;
    std::string Input_;
    int ResultID_ = 0;
    int GraphID_ = 0;

    std::unordered_map<std::string, TTask> Tasks_;                                                                                                                                                             
    std::unordered_map<std::string, std::string> Results_;
    std::unordered_map<std::string, int> InputNodes_;

    const std::string START_NODE = "START";
    const std::string FINISH_NODE = "FINISH";
};
