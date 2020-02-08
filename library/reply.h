#pragma once
#include <string>
#include <vector>

struct TReply {
    enum class StatusType {
        OK = 200,
        NOT_FOUND = 404,
    } Status;
    std::string Content;
    std::vector<std::pair<std::string, std::string>> Headers;
    std::string Serialize();
};

