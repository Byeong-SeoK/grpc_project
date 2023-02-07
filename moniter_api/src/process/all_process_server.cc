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

using moniter::ProcessMoniterReply;
using moniter::ProcessMoniterRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::current_process_moniter_method(ServerContext *context,
                                                          const ProcessMoniterRequest *request,
                                                          ProcessMoniterReply *reply)
{
    SetlogDir dir;
    std::string logDir = dir.setDir();
    mkdir(logDir.c_str(), 0755); // 날짜별 로그 폴더 생성, 폴더 접근 권한은 0755가 가장 기본적인 값이다.
    FLAGS_log_dir = logDir;

    LOG(INFO) << "Total process monitoring API start .";

    char buffer[128];                  // pipe를 통해서 shell창의 출력 결과를 임시적으로 저장하는 buffer이다.
    std::string all_process_info = ""; // shell창의 출력 결과를 받는 변수
    FILE *pipe = popen("ps -ef", "r"); // shell창에 ps라는 명령어를 내리고 그 결과를 read mode로 읽어온다.
    if (!pipe)
    {
        LOG(ERROR) << "popen() failed!";
        throw std::runtime_error("popen() failed!");
    }
    try
    {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) // buffer에 저장된 것 끝까지 하나씩 읽어서 all_process_info에 저장한다.
        {
            all_process_info += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        return grpc::Status(grpc::StatusCode::DEADLINE_EXCEEDED, "popen() failed!");
    }

    std::string process_pid(std::to_string(getpid()));         // 현재 process의 PID를 받는다.
    std::string parent_process_pid(std::to_string(getppid())); // 현재 process의 parent process의 PID를 받는다.

    reply->set_process_info_reply(request->process_info_request() + process_pid + "\n" + request->parent_process_info_request() +
                                  parent_process_pid + "\n" + request->all_process_info_request() + "\n" + all_process_info);

    LOG(INFO) << "Total process monitoring API end .";

    return Status::OK;
}