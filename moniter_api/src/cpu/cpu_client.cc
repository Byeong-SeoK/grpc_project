#include "../../include/moniter_client.h"
#include "../../include/save_log.h"

#include <iostream>
#include <memory>
#include <string>
#include <glog/logging.h>

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

std::string MoniterClient::current_cpu_usage_moniter_method(const std::string &sentence)
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

    SaveLog log;

    // Act upon its status.
    if (status.ok())
    {
        // LOG(INFO) << "CPU monitoring service API success" << std::endl;
        log.save_level_Log(google::INFO, "CPU monitoring service API success");
        return reply.cpu_reply();
    }
    else
    {
        // LOG(ERROR) << status.error_code() << ": " << status.error_message() << std::endl;
        std::string error_msg = status.error_code() + ": " + status.error_message();
        log.save_level_Log(google::ERROR, error_msg.c_str());
        return "RPC failed";
    }
}