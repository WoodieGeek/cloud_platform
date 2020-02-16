#pragma once
#include <string>
#include <vector>


struct TReply {
    enum class EStatusType {
        OK = 200,
        NOT_FOUND = 404,
    };
    TReply() = default;
    TReply(const std::string& content, EStatusType status);
    EStatusType Status;
    std::string Content;
    std::vector<std::pair<std::string, std::string>> Headers;
    std::string Serialize();
};

