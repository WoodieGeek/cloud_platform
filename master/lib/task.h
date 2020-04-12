#pragma once
#include <string>
#include <vector>

struct TFile {
    std::string Name;
    std::string Content;
};

struct TTask {
    std::vector<TFile> Inputs;
    std::string Binary;
};
