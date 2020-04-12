#include "reply_parser.h"
namespace {
    std::vector<std::string_view> Split(std::string_view data, std::string_view delim) {
        std::vector<std::string_view> result;
        size_t pos = 0;
        size_t last = 0;
        while ((pos = data.find(delim, pos)) != std::string::npos) {
            result.emplace_back(data.substr(last, pos - last));
            pos += delim.size();
            last = pos;
        }
        if (last < data.size()) {
            result.emplace_back(data.substr(last, data.size() - last));
        }
        return result;
    }

    void ParseFirstLine(std::string_view line, TReply& result) {
        const auto& splitBySpace = Split(line, " ");
        if (splitBySpace.size() != 3) {
            return;
        }
        switch (stoi(std::string{splitBySpace[1]})) {
            case 200: {
                result.Status = TReply::EStatusType::OK;
                break;
            }
            default: {
                result.Status = TReply::EStatusType::NOT_FOUND;
            }
        }
    }
};


std::optional<TReply> TReplyParser::Parse(std::string_view data) {
    std::string_view delim = "\r\n";
    const auto& tokens = Split(data, delim);
    if (tokens.empty()) {
        return {};
    }
    TReply result;
    ParseFirstLine(tokens[0], result);
    for (size_t i = 1; i < tokens.size() - 1; ++i) {
        const auto& splitHeader = Split(tokens[i], ":");
        if (splitHeader.size() != 2) {
            continue;
        }
        result.Headers.emplace_back(splitHeader[0], splitHeader[1]);
    }
    result.Content = tokens.back();
    return result;
};
