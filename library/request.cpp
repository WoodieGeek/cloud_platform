#include "request.h"
#include <sstream>

std::string TRequest::Serialize() const {
    std::stringstream result;
    result << Method << " " << Uri;
    if (!Cgi.empty()) {
        result << "?";
        for (const auto& [key, value] : Cgi) {
            result << key << "=" << value << "&";
        }
    }
    result << " HTTP/1.1\r\n";
    for (const auto& [key, value] : Headers) {
        result << key << ": " << value << "\r\n";
    }
    result << "\r\n";
    return result.str();
}
