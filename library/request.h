#pragma once

#include <string>
#include <vector>

struct TRequest {
    std::string Method;
    std::string Uri;
    std::vector<std::pair<std::string, std::string>> Headers;
    std::vector<std::pair<std::string, std::string>> Cgi;
    std::string Content;
};
