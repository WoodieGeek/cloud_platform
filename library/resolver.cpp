#include "resolver.h"

#include <iostream>

TResolver::TResolver() {

}

std::string TResolver::Resolve(TRequest request) const {
    if (ResolveHandle_.count(request.Uri_)) {
        return ResolveHandle_.at(request.Uri_)(request);
    } else {
        return "bad uri";
    }
}

void TResolver::AddHander(const std::string& nameHandler, const std::function<std::string(TRequest)>& handler) {
    ResolveHandle_[nameHandler] = handler;
}

