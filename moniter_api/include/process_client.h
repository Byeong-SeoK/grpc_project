#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using moniter::ProcessMoniterReply;
using moniter::ProcessMoniterRequest;

using moniter::SelectedProcessMoniterReply;
using moniter::SelectedProcessMoniterRequest;

using moniter::MoniterService;

class ProcessMoniterClient
{
public:
    ProcessMoniterClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}
    std::string current_process_moniter_method(const std::string &process_info_request,
                                               const std::string &parent_process_info_request,
                                               const std::string &all_process_info_request);

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};

class SelectedProcessMoniterClient
{
public:
    SelectedProcessMoniterClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}

    std::string selected_process_moniter_method(const std::string &selected_process_name,
                                                const std::string &pid_process_info_request);

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};