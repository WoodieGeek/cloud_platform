#include "subprocess.h"
#include <iostream>
#include <fstream>
#include <sstream>

TSubProcess::TSubProcess(const std::string& init)
    : Child_(init) {
}

void TSubProcess::Stop() {
    if (Child_.running()) {
        Child_.terminate();
    }
}

bool TSubProcess::IsRunning() {
    return Child_.running();
}

std::string TSubProcess::GetResult() const {
    std::ifstream file("out.txt");
    std::stringstream result;
    if (file.is_open()) {
        result << file.rdbuf();
    }
    return result.str();
}
