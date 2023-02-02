#include <glog/logging.h>

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

using moniter::ServiceLogReply;
using moniter::ServiceLogRequest;

using moniter::MoniterService;

class ServiceLogMonitor
{
public:
    ServiceLogMonitor(std::shared_ptr<Channel> channel) : stub_(MoniterService::NewStub(channel)) {}
    std::string service_log_monitor_method(const std::string &service_log_request, char **temp);
    char **simple_transmission(char **temp); // google::InitGoogleLogging(argv[0]);에서 argv[0]값을 grpc method에 전달하기 위한 함수

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};