#include "../../include/moniter_client.h"
#include "../../include/set_log_dir.h"

#include <sys/stat.h>
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

    SetlogDir dir;
    std::string logDir = dir.setDir();
    mkdir(logDir.c_str(), 0755); // 날짜별 로그 폴더 생성, 폴더 접근 권한은 0755가 가장 기본적인 값이다.
    FLAGS_log_dir = logDir;

    // Act upon its status.
    if (status.ok())
    {
        LOG(INFO) << "Disk monitoring service API Success . ";
        google::FlushLogFiles(google::GLOG_INFO);
        return reply.disk_info_reply();
    }
    else
    {
        LOG(ERROR) << status.error_code() << ": " << status.error_message();
        google::FlushLogFiles(google::GLOG_ERROR);
        return "RPC failed";
    }
}