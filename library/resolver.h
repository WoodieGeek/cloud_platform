#pragma once

#include "request.h"
#include "reply.h"

#include <unordered_map>
#include <string>
#include <functional>

class TResolver {
public:
    TResolver();
    TReply Resolve(TRequest request) const;
    void AddHander(const std::string& nameHandler, const std::function<TReply(TRequest)>& handler);
private:
    std::unordered_map<std::string, std::function<TReply(TRequest)>> ResolveHandle_;
};
