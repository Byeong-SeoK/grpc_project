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

using moniter::DiskMoniterReply;
using moniter::DiskMoniterRequest;

using moniter::MoniterService;

class DiskMoniterClient
{
public:
    DiskMoniterClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}
    std::string current_disk_usage_moniter_method(const std::string &total_disk_volume_request,
                                                  const std::string &disk_usage_request,
                                                  const std::string &avail_disk_volume_request);

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};