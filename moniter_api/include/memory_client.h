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

using moniter::MemoryReply;
using moniter::MemoryRequest;

using moniter::MoniterService;

class MemoryClient
{
public:
    MemoryClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string current_memory_moniter_method(const std::string virtual_memory_request,
                                              const std::string physical_memory_request,
                                              const std::string avail_virtual_memory_request,
                                              const std::string avail_physical_memory_request);

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};