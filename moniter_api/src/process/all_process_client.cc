#include "../../include/moniter_client.h"

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

using moniter::MoniterService;

std::string MoniterClient::current_process_moniter_method(const std::string &process_info_request,
                                                          const std::string &parent_process_info_request,
                                                          const std::string &all_process_info_request) // 모든 process 정보 조회
{
    ProcessMoniterRequest request;
    request.set_process_info_request(process_info_request);
    request.set_parent_process_info_request(parent_process_info_request);
    request.set_all_process_info_request(all_process_info_request);

    ProcessMoniterReply reply;

    ClientContext context;

    // The actual RPC.
    Status status =
        stub_->current_process_moniter_method(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
        return reply.process_info_reply();
    }
    else
    {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
    }
}