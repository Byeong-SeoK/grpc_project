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
#include "fstream"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::ServiceLogReply;
using moniter::ServiceLogRequest;

using moniter::MoniterService;

void MoniterServiceImpl::Write(const std::string &pgmname, const std::string &msg)
{
    std::string DirLog = "../../file";
    std::string strDataTime = "";
    std::string strLogFileName = DirLog + pgmname + ".log";
    std::string strMessage = strDataTime + "," + msg + "\n";

    // ofstream ofs(filename.c_str(), ios::out | ios::app);
    // if (ofs.fail())
    // {
    //     cerr << filename << "couldn't open file!!" << std::endl;
    //     throw StreamException("CLogManager::Write - " + filename);
    // }

    // ofs << msg;

    // ofs.clear();
    // ofs.close();
}

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