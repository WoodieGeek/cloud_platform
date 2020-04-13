#pragma once
#include "instances.h"
#include "graph.h"
#include <pqxx/pqxx>

class TGraphRunner {
public:
    TGraphRunner(pqxx::connection& connection, TInstancesHolder& instancesHolder);
    void RunGraph(int graphID, std::string input);
private:
    TGraph GetGraph();
private:
    int GraphID_;
    std::string Input;
    pqxx::connection& Connection_;
    TInstancesHolder& InstancesHolder_;
    std::unordered_map<std::string, TTask> Tasks_;
    std::unordered_map<std::string, std::string> Results_;
};
