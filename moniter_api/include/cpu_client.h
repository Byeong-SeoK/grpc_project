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

using moniter::CpuMoniterReply;
using moniter::CpuMoniterRequest;

using moniter::MoniterService;

class CpuMoniterClient
{
public:
    CpuMoniterClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}
    std::string current_cpu_usage_moniter_method(const std::string &sentence);

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};