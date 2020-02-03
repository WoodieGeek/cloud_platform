#pragma once

#include <string>
#include <vector>

struct TRequest {
    std::string Method_;
    std::string Uri_;
    std::vector<std::pair<std::string, std::string>> Headers_;
    std::vector<std::pair<std::string, std::string>> Cgi_;
};
