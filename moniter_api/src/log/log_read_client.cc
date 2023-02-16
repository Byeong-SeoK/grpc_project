#include "../../include/moniter_client.h"

#include "sys/stat.h"
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

using moniter::LogReply;
using moniter::LogRequest;

using moniter::MoniterService;

std::string MoniterClient::client_log_monitor_method(const std::string &LogDate, const std::string &comType, const std::string &LogType)
{
    // Data we are sending to the server.
    LogRequest request;
    request.set_log_date_request(LogDate);
    request.set_log_com_request(comType);
    request.set_log_type_request(LogType);

    // Container for the data we expect from the server.
    LogReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status =
        stub_->server_log_monitor_method(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
        LOG(INFO) << "Log monitoring service API Success";
        return reply.log_reply();
    }
    else
    {
        LOG(ERROR) << status.error_code() << ": " << status.error_message();
        return "RPC failed";
    }
}