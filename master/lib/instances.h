#pragma once
#include "task.h"
#include "../../library/client.h"

#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

struct TTaskResult {
    std::string Result;
};

struct THost {
    std::string ip;
    unsigned int port;
};

class TInstancesHolder {
public:
    TInstancesHolder(boost::asio::io_service& ioService);
    void Run(TTask task, std::function<void(const TTaskResult&)> callback);
    void Stop();
private:
    void RunProcess(const THost& host, const TTask& task, std::function<void(const TTaskResult&)> callback);
    void PingProcess(const THost& host, std::function<void(const TTaskResult&)> callback);
    void AnswerProcess(const THost& host, std::function<void(const TTaskResult&)> callback);
private:
    std::vector<THost> Instances_;
    std::vector<std::thread> Threads_;
    std::queue<std::pair<TTask, std::function<void(const TTaskResult&)>>> Tasks_;
    std::condition_variable Condition_;
    std::mutex QueueMutex_;
    std::atomic_bool Active_{true};
    TClient Client_;
};

