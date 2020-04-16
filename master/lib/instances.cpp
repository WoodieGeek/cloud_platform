#include "instances.h"
#include <nlohmann/json.hpp>
#include <iostream>


TInstancesHolder::TInstancesHolder(boost::asio::io_service& ioService) : Client_(ioService) {
    Instances_.push_back({"127.0.0.1", 10001});
    Instances_.push_back({"127.0.0.1", 10002});
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
    std::cerr << "Run Process\n";
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
    Client_.HttpRequest(host.ip, host.port, request, [this, host, callback] (const TReply& reply) {
        if (reply.Status == TReply::EStatusType::OK) {
            PingProcess(host, callback);
        }
    });
}

void TInstancesHolder::PingProcess(const THost& host, std::function<void(const TTaskResult&)> callback) {
    TRequest request;
    request.Method = "GET";
    request.Uri = "/is_running";
    Client_.HttpRequest(host.ip, host.port, request, [this, host, callback] (const TReply& reply) {
        if (reply.Status == TReply::EStatusType::OK) {
            if (reply.Content  == "true") {
                AnswerProcess(host, callback);
            } else if (reply.Content == "false") {
                PingProcess(host, callback);
            }
        }
    });
}

void TInstancesHolder::AnswerProcess(const THost& host, std::function<void(const TTaskResult&)> callback) {
    TRequest request;
    request.Method = "GET";
    request.Uri = "/get_response";
    Client_.HttpRequest(host.ip, host.port, request, [callback] (const TReply& reply) {
        if (reply.Status == TReply::EStatusType::OK) {
            TTaskResult result;
            result.Result = reply.Content;
            callback(result);
        }
    });
}


