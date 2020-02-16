#include "resolver.h"
#include "reply.h"
#include <iostream>

TResolver::TResolver() {

}

TReply TResolver::Resolve(TRequest request) const {
    if (ResolveHandle_.count(request.Uri)) {
        return ResolveHandle_.at(request.Uri)(request);
    } else {
        return {"bad url", TReply::EStatusType::NOT_FOUND};
    }
}

void TResolver::AddHander(const std::string& nameHandler, const std::function<TReply(TRequest)>& handler) {
    ResolveHandle_[nameHandler] = handler;
}

