#include "../../include/moniter_client.h"
#include <glog/logging.h>

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::ServiceLogReply;
using moniter::ServiceLogRequest;

using moniter::MoniterService;

std::string MoniterClient::service_log_monitor_method(const std::string &service_log_request)
{

    // Data we are sending to the server.
    ServiceLogRequest request;
    request.set_service_log_request(service_log_request);

    // Container for the data we expect from the server.
    ServiceLogReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status =
        stub_->service_log_monitor_method(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
        return reply.service_log_reply();
    }
    else
    {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
    }
}