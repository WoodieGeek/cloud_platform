#pragma once
#include "instances.h"

struct TServerOptions {
    std::vector<THost> Instances;
    static TServerOptions Parse(const std::string& data);
};
