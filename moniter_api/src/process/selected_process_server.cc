#include "../../include/moniter_server.h"
#include "../../include/set_log_dir.h"

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"
#include "sys/stat.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::SelectedProcessMoniterReply;
using moniter::SelectedProcessMoniterRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::selected_process_moniter_method(ServerContext *context,
                                                           const SelectedProcessMoniterRequest *request,
                                                           SelectedProcessMoniterReply *reply)
{
    SetlogDir dir;
    std::string logDir = dir.setDir();
    mkdir(logDir.c_str(), 0755); // 날짜별 로그 폴더 생성, 폴더 접근 권한은 0755가 가장 기본적인 값이다.
    FLAGS_log_dir = logDir;

    LOG(INFO) << "Selected process monitoring API start .";

    char pid_buffer[128];
    std::string pid_process_info = "";
    std::string temp = "ps -ef | grep " + request->selected_process_name(); // request
    const char *command = temp.c_str();
    FILE *pipe2 = popen(command, "r");
    if (!pipe2)
    {
        LOG(ERROR) << "popen() failed!";
        throw std::runtime_error("popen() failed!");
    }
    try
    {
        while (fgets(pid_buffer, sizeof(pid_buffer), pipe2) != NULL)
        {
            pid_process_info = pid_process_info + pid_buffer;
        }
    }
    catch (...)
    {
        pclose(pipe2);
        return grpc::Status(grpc::StatusCode::DEADLINE_EXCEEDED, "popen() failed!");
    }

    reply->set_selected_process_info_reply(request->pid_process_info_request() + pid_process_info);

    LOG(INFO) << "Selected process monitoring API end .";

    return Status::OK;
}