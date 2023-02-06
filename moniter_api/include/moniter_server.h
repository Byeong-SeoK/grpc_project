#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <glog/logging.h>
#include <gflags/gflags.h>

#ifdef BAZEL_BUILD
#include "moniter_api/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::MemoryReply;
using moniter::MemoryRequest;

using moniter::CpuMoniterReply;
using moniter::CpuMoniterRequest;

using moniter::DiskMoniterReply;
using moniter::DiskMoniterRequest;

using moniter::ProcessMoniterReply;
using moniter::ProcessMoniterRequest;

using moniter::SelectedProcessMoniterReply;
using moniter::SelectedProcessMoniterRequest;

using moniter::ServiceLogReply;
using moniter::ServiceLogRequest;

using moniter::MoniterService;

// Logic and data behind the server's behavior.
class MoniterServiceImpl final : public MoniterService::Service
{
    struct stJiffies // 실시간 cpu 사용량 측정을 위한 구조체
    {
        int user;
        int nice;
        int system;
        int idle;
        double usage = 0.0;
    };
    stJiffies curJiffies;
    stJiffies prevJiffies;

    Status current_memory_moniter_method(ServerContext *context, const MemoryRequest *request, MemoryReply *reply) override;

    Status current_cpu_usage_moniter_method(ServerContext *context, const CpuMoniterRequest *request, CpuMoniterReply *reply) override;

    Status current_disk_usage_moniter_method(ServerContext *context, const DiskMoniterRequest *request, DiskMoniterReply *reply) override;

    Status current_process_moniter_method(ServerContext *context, const ProcessMoniterRequest *request, ProcessMoniterReply *reply) override;

    Status selected_process_moniter_method(ServerContext *context, const SelectedProcessMoniterRequest *request, SelectedProcessMoniterReply *reply) override;

    Status service_log_monitor_method(ServerContext *context, const ServiceLogRequest *request, ServiceLogReply *reply) override;

    // void writeLog(std::string name, int line, int lv, const char *str, ...);
};

// class SaveLog
// {
// public:
//     void save_level_Log(const char *log, int level);
// };