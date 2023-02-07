#include "../../include/moniter_client.h"
#include "../../include/set_log_dir.h"

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

    SetlogDir dir;
    std::string logDir = dir.setDir();
    mkdir(logDir.c_str(), 0755); // 날짜별 로그 폴더 생성, 폴더 접근 권한은 0755가 가장 기본적인 값이다.
    FLAGS_log_dir = logDir;

    // Act upon its status.
    if (status.ok())
    {
        LOG(INFO) << "CPU monitoring service API success";
        return reply.cpu_reply();
    }
    else
    {
        LOG(ERROR) << status.error_code() << ": " << status.error_message();
        return "RPC failed";
    }
}