#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct TRequest {
    std::string Method;
    std::string Uri;
    std::vector<std::pair<std::string, std::string>> Headers;
    std::unordered_multimap<std::string, std::string> Cgi;
    std::string Content;
    std::string Serialize() const;
};
