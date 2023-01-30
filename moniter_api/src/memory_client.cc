#include "../include/memory_client.h"

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

std::string MemoryClient::current_memory_moniter_method(const std::string virtual_memory_request,
                                                        const std::string physical_memory_request,
                                                        const std::string avail_virtual_memory_request,
                                                        const std::string avail_physical_memory_request)
{
    // Data we are sending to the server.
    MemoryRequest request;
    request.set_virtual_memory_request(virtual_memory_request);
    request.set_physical_memory_request(physical_memory_request);
    request.set_avail_virtual_memory_request(avail_virtual_memory_request);
    request.set_avail_physical_memory_request(avail_physical_memory_request);

    // Container for the data we expect from the server.
    MemoryReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status =
        stub_->current_memory_moniter_method(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
        return reply.memory_info_reply();
    }
    else
    {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
    }
}