#include "resolver.h"

#include <iostream>

TResolver::TResolver() {
    auto tmp = [](TRequest ) {
        return "index ans";
    };
    ResolveHandle_["/index"] = tmp;
}

std::string TResolver::Resolve(TRequest request) const {
    if (ResolveHandle_.count(request.Uri_)) {
        return ResolveHandle_.at(request.Uri_)(request);
    } else {
        return "bad uri";
    }
}

