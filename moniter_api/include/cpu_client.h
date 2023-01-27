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
    std::string current_cpu_usage_moniter_method(const std::string &sentence)
    {
        // Data we are sending to the server.
        CpuMoniterRequest request;
        request.set_cpu_request(sentence);

        // Container for the data we expect from the server.
        CpuMoniterReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status =
            stub_->current_cpu_usage_moniter_method(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            return reply.cpu_reply();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};