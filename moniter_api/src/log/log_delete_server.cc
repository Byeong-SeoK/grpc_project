#include "../../include/moniter_server.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <typeinfo>
#include <unistd.h>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"
#include "sys/stat.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::LogDeleteReply;
using moniter::LogDeleteRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::server_log_delete_method(ServerContext *context, const LogDeleteRequest *request, LogDeleteReply *reply)
{
    std::string dirPath = "/mnt/c/Users/INNO-C-535/grpc_project/log_data/";
    std::string delete_file_path = dirPath + request->log_delete_request() + ".txt";
    delete_file_path.erase(std::remove_if(delete_file_path.begin(), delete_file_path.end(), isspace), delete_file_path.end());

    bool result = unlink(delete_file_path.c_str()); // 0이면 올바르게 삭제, -1이면 error 발생

    if (!result)
    {
        reply->set_log_delete_reply(request->log_delete_request() + ".txt" + " deleted!!!");
    }
    else
    {
        LOG(ERROR) << "log file opened failed!";
        std::cerr << "Error: " << strerror(errno) << std::endl; // 어떤 에러가 발생하였는지 확인
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Cannot find the directory");
    }

    return Status::OK;
}