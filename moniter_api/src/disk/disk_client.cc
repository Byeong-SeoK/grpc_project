#include "../../include/moniter_client.h"
#include "../../include/save_log.h"

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

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

std::string MoniterClient::current_disk_usage_moniter_method(const std::string &total_disk_volume_request,
                                                             const std::string &disk_usage_request,
                                                             const std::string &avail_disk_volume_request)
{

    // Data we are sending to the server.
    DiskMoniterRequest request;
    request.set_total_disk_volume_request(total_disk_volume_request);
    request.set_disk_usage_request(disk_usage_request);
    request.set_avail_disk_volume_request(avail_disk_volume_request);

    // Container for the data we expect from the server.
    DiskMoniterReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status =
        stub_->current_disk_usage_moniter_method(&context, request, &reply);

    SaveLog log;
    // Act upon its status.
    if (status.ok())
    {
        // LOG(INFO) << "Disk monitoring service API Success" << std::endl;
        log.save_level_Log(google::INFO, "Disk monitoring service API Success");
        return reply.disk_info_reply();
    }
    else
    {
        // LOG(ERROR) << status.error_code() << ": " << status.error_message() << std::endl;
        std::string error_msg = status.error_code() + ": " + status.error_message();
        log.save_level_Log(google::ERROR, error_msg.c_str());

        return "RPC failed";
    }
}