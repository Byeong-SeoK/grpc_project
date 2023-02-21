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

using moniter::LogDeleteReply;
using moniter::LogDeleteRequest;

using moniter::MoniterService;

std::string MoniterClient::client_log_delete_method(const std::string &filename)
{
    // Data we are sending to the server.
    LogDeleteRequest request;
    request.set_log_delete_request(filename);

    // Container for the data we expect from the server.
    LogDeleteReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status =
        stub_->server_log_delete_method(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
        LOG(INFO) << "Log delete service API Success . ";
        return reply.log_delete_reply();
    }
    else
    {
        LOG(ERROR) << status.error_code() << ": " << status.error_message();
        return "RPC failed";
    }
}