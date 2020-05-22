#include "instances.h"
#include <nlohmann/json.hpp>
#include <iostream>


TInstancesHolder::TInstancesHolder(boost::asio::io_service& ioService, std::vector<THost> instances) 
    : Instances_(std::move(instances))
    , Client_(ioService) {
    const auto threadCount = Instances_.size();
    Threads_.reserve(threadCount);
    for (size_t i = 0; i < threadCount; ++i) {
        Threads_.emplace_back([this, host = Instances_[i]] {
            while (this->Active_) {
                std::optional<TTask> task;
                std::function<void(const TTaskResult&)> callback;
                {
                    std::unique_lock<std::mutex> lock(this->QueueMutex_);
                    this->Condition_.wait(lock, [this] {
                        return this->Active_ && !this->Tasks_.empty();
                    });
                    if (!Tasks_.empty()) {
                        task = std::move(this->Tasks_.front().first);
                        callback = std::move(this->Tasks_.front().second);
                        Tasks_.pop();
                    }
                }
                if (task.has_value()) {
                    RunProcess(host, *task, callback);
                }
            }
        });
    }
}

void TInstancesHolder::Run(TTask task, std::function<void(const TTaskResult&)> callback) {
    Tasks_.push({std::move(task), std::move(callback)});
    Condition_.notify_one();
}

void TInstancesHolder::Stop() {
    Active_ = false;
    for (auto& thread : Threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void TInstancesHolder::RunProcess(const THost& host, const TTask& task, std::function<void(const TTaskResult&)> callback) {
    TRequest request;
    request.Method = "POST";
    request.Uri = "/run";
    request.Headers.push_back({"Content-Type", "application/json"});
    nlohmann::json content;
    content["binary"] = task.Binary;
    nlohmann::json inputs;
    for (const auto& file : task.Inputs) {
        nlohmann::json fileJson;
        fileJson["name"] = file.Name;
        fileJson["content"] = file.Content;
        inputs.push_back(std::move(fileJson));
    }
    content["inputs"] = std::move(inputs);
    request.Content = content.dump();
    auto reply = Client_.HttpRequest(host.ip, host.port, request);
    if (reply.Status == TReply::EStatusType::OK) {
        PingProcess(host, callback);
    }
}

void TInstancesHolder::PingProcess(const THost& host, std::function<void(const TTaskResult&)> callback) {
    TRequest request;
    request.Method = "GET";
    request.Uri = "/is_running";
    auto reply = Client_.HttpRequest(host.ip, host.port, request);
    if (reply.Status == TReply::EStatusType::OK) {
        if (reply.Content == "false") {
            AnswerProcess(host, callback);
        } else if (reply.Content == "true") {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            PingProcess(host, callback);
        }
    }
}

void TInstancesHolder::AnswerProcess(const THost& host, std::function<void(const TTaskResult&)> callback) {
    TRequest request;
    request.Method = "GET";
    request.Uri = "/get_response";
    auto reply = Client_.HttpRequest(host.ip, host.port, request);
    if (reply.Status == TReply::EStatusType::OK) {
        TTaskResult result;
        result.Result = reply.Content;
        callback(result);
    }
}


