#include "graph_runner.h"

TGraphRunner::TGraphRunner(pqxx::connection& connection, TInstancesHolder& instancesHolder) 
    : Connection_(connection)
    , InstancesHolder_(instancesHolder)
{
}

void TGraphRunner::RunGraph(int graphID, std::string input) {
    GraphID_ = graphID;
    Input = input;
    auto graph = GetGraph();
}
