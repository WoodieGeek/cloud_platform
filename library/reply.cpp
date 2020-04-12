#include "reply.h"

#include <sstream>
#include <iostream>

TReply::TReply(const std::string& content, EStatusType status)
    : Status(status)
    , Content(content)
{
}

std::string TReply::Serialize() {
    std::stringstream result;
    switch (Status) {
        case EStatusType::OK:{
            result << "HTTP/1.0 200 OK\r\n\r\n";
            break;
        }
        case EStatusType::NOT_FOUND: {
            result << "HTTP/1.0 404 Not Found\r\n\r\n";
            break;
        }
    }
    for (const auto& [header, value] : Headers) {
        result << header << ": " << value << '\n';
    }
    result << Content;
    return result.str();
};

