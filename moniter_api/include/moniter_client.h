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

using moniter::MemoryReply;
using moniter::MemoryRequest;

using moniter::DiskMoniterReply;
using moniter::DiskMoniterRequest;

using moniter::CpuMoniterReply;
using moniter::CpuMoniterRequest;

using moniter::ProcessMoniterReply;
using moniter::ProcessMoniterRequest;

using moniter::SelectedProcessMoniterReply;
using moniter::SelectedProcessMoniterRequest;

using moniter::ServiceLogReply;
using moniter::ServiceLogRequest;

using moniter::MoniterService;

class MoniterClient
{
public:
    MoniterClient(std::shared_ptr<Channel> channel) : stub_(MoniterService::NewStub(channel)) {}

    std::string current_memory_moniter_method(const std::string virtual_memory_request,
                                              const std::string physical_memory_request,
                                              const std::string avail_virtual_memory_request,
                                              const std::string avail_physical_memory_request);

    std::string current_disk_usage_moniter_method(const std::string &total_disk_volume_request,
                                                  const std::string &disk_usage_request,
                                                  const std::string &avail_disk_volume_request);

    std::string current_cpu_usage_moniter_method(const std::string &sentence);

    std::string current_process_moniter_method(const std::string &process_info_request,
                                               const std::string &parent_process_info_request,
                                               const std::string &all_process_info_request);

    std::string selected_process_moniter_method(const std::string &selected_process_name, const std::string &pid_process_info_request);

    std::string service_log_monitor_method(const std::string &service_log_request);

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};