#pragma once

#include "request.h"

#include <unordered_map>
#include <string>
#include <functional>

class TResolver {
public:
    TResolver();
    std::string Resolve(TRequest request) const;
private:
    std::unordered_map<std::string, std::function<std::string(TRequest)>> ResolveHandle_;
};
