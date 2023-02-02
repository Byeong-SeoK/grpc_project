#include "../../include/moniter_server.h"

#include <glog/logging.h>

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"
#include "time.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::ServiceLogReply;
using moniter::ServiceLogRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::service_log_monitor_method(ServerContext *context, const ServiceLogRequest *request, ServiceLogReply *reply)
{
    time_t currentSec = time(NULL);
    tm *t = localtime(&currentSec);
    // std::cout << t->tm_mon + 1 << " " << t->tm_mday << " " << t->tm_hour << " " << t->tm_min << " " << t->tm_sec << std::endl;
    LOG(INFO) << "INFO 레벨의 로그";
    LOG(WARNING) << "WARNING 레벨의 로그";
    LOG(ERROR) << "ERROR 레벨의 로그";
    sleep(1);
    reply->set_service_log_reply(request->service_log_request());
    return Status::OK;
}