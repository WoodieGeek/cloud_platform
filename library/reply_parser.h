#pragma once
#include "reply.h"
#include <optional>

class TReplyParser {
public:
    static std::optional<TReply> Parse(std::string_view data);
};
