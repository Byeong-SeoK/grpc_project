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

using moniter::SelectedProcessMoniterReply;
using moniter::SelectedProcessMoniterRequest;

using moniter::MoniterService;

std::string MoniterClient::selected_process_moniter_method(const std::string &selected_process_name,
                                                           const std::string &pid_process_info_request)
{
    SelectedProcessMoniterRequest request;
    request.set_pid_process_info_request(pid_process_info_request);
    request.set_selected_process_name(selected_process_name);

    SelectedProcessMoniterReply reply;

    ClientContext context;

    // The actual RPC.
    Status status =
        stub_->selected_process_moniter_method(&context, request, &reply);

    SetlogDir dir;
    std::string logDir = dir.setDir();
    mkdir(logDir.c_str(), 0755); // 날짜별 로그 폴더 생성, 폴더 접근 권한은 0755가 가장 기본적인 값이다.
    FLAGS_log_dir = logDir;

    // Act upon its status.
    if (status.ok())
    {
        LOG(INFO) << "Selected process monitoring service API Success";
        return reply.selected_process_info_reply();
    }
    else
    {
        LOG(ERROR) << status.error_code() << ": " << status.error_message();
        return "RPC failed";
    }
}