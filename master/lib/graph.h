#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

struct TGraph { 
    TGraph(const std::string& graph);
    std::unordered_map<std::string, std::vector<std::string>> Graph_;
};
