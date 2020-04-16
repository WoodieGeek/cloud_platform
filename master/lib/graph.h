#include <string>
#include <vector>
#include <unordered_map>

struct TGraph { 
    TGraph() = default;
    TGraph(const std::string& graph);
    std::unordered_map<std::string, std::vector<std::string>> Graph;
};
