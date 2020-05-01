#include "graph.h"
#include <nlohmann/json.hpp>

TGraph::TGraph(const std::string& graph) {
    auto graphJson = nlohmann::json::parse(graph);
    for (const auto& [key, value] : graphJson.items()) {
        for (const auto& node : value) {
            Graph[key].push_back(node.get<std::string>());
        }
    }
}
