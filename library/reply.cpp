#include "reply.h"
#include <sstream>

std::string TReply::Serialize() {
    std::stringstream result;
    switch (Status) {
        case StatusType::OK:{
            result << "HTTP/1.0 200 OK\r\n";
            break;
        }
        case StatusType::NOT_FOUND: {
            result << "HTTP/1.0 404 Not Found\r\n";
            break;
        }
    }
    for (const auto& [header, value] : Headers) {
        result << header << ": " << value << '\n';
    }
    result << Content;
    return result.str();
};
