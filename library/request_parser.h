#pragma once
#include "request.h"
#include <optional>

struct TRequestParser {
    static std::optional<TRequest> Parse(const std::string_view data);
};
