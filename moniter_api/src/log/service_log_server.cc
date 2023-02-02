#include "../../include/moniter_server.h"

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

Status MoniterServiceImpl::service_log_monitor_method(ServerContext *context, const ServiceLogRequest *request, ServiceLogReply *reply, char **temp)
{
    google::InitGoogleLogging(temp[0]);

    // default 설정 시, INFO, WARNING 레벨은 로그 파일에만 출력됨
    // LOG(INFO) << "INFO 레벨의 로그";
    LOG(WARNING) << "WARNING 레벨의 로그";

    // default 설정 시, ERROR 레벨 이상부터 stderr 로 출력된다.
    // LOG(ERROR) << "ERROR 레벨의 로그";

    // FATAL의 경우, Stack trace를 출력하고 프로그램을 종료시킨다.
    // LOG(FATAL) << "FATAL 레벨의 로그"

    reply->set_service_log_reply(request->service_log_request() + LOG(WARNING) << "INFO 레벨의 로그");
    return Status::OK;
}