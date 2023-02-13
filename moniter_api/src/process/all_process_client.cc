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

    SetlogDir dir;
    std::string logDir = dir.setDir();

    // Act upon its status.
    if (status.ok())
    {
        LOG(INFO) << "Total process monitoring service API Success";
        google::FlushLogFiles(google::GLOG_INFO);
        return reply.process_info_reply();
    }
    else
    {
        LOG(ERROR) << status.error_code() << ": " << status.error_message();
        google::FlushLogFiles(google::GLOG_ERROR);
        return "RPC failed";
    }
}