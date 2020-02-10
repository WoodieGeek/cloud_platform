#include "request_parser.h"
#include <boost/algorithm/string.hpp>


#include <iostream>

namespace {
    void AddUriAndCgi(std::string_view data, TRequest result) {
        auto endUri = data.find_first_of('?');
        result.Uri_ = data.substr(0, endUri);
        std::vector<std::string> cgiVector;
        std::string cgiString(data.substr(endUri + 1, data.size()));
        boost::split(cgiVector, cgiString, [](char c) {return c == '&';});
        for (auto& cgi : cgiVector) {
            std::vector<std::string> key_value;
            boost::split(key_value, cgi, [](char c) {return c == '=';});
            if (key_value.size() != 2) {
                continue;
            }
            result.Cgi_.emplace_back(std::move(key_value[0]), std::move(key_value[1]));
        }
    }
}


std::optional<TRequest> TRequestParser::Parse(const std::string_view data) {
    std::vector<std::string> lines;
    boost::split(lines, data, [](char c) {return c == '\n';});
    if (lines.empty()) {
        return {};
    }
    std::vector<std::string> first_line;
    boost::split(first_line, lines.front(), [](char c) {return c == ' ';});
    if (first_line.size() != 3) {
        return {};
    }
    TRequest result;
    result.Method_ = std::move(first_line[0]);
    AddUriAndCgi(first_line[1], result);
    for (size_t i = 1; i < lines.size(); i++) {
        std::vector<std::string> key_value;
        boost::split(key_value, lines[i], boost::is_any_of(": "));
        if (key_value.size() != 2) {
            continue;
        }
        result.Headers_.emplace_back(std::move(key_value[0]), std::move(key_value[1]));
    }
    return {result};
}
