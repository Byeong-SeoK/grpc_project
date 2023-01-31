#include "../../include/moniter_server.h"

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

using moniter::ProcessMoniterReply;
using moniter::ProcessMoniterRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::current_process_moniter_method(ServerContext *context,
                                                          const ProcessMoniterRequest *request,
                                                          ProcessMoniterReply *reply)
{
    char buffer[128];                  // pipe를 통해서 shell창의 출력 결과를 임시적으로 저장하는 buffer이다.
    std::string all_process_info = ""; // shell창의 출력 결과를 받는 변수
    FILE *pipe = popen("ps -ef", "r"); // shell창에 ps라는 명령어를 내리고 그 결과를 read mode로 읽어온다.
    if (!pipe)
        throw std::runtime_error("popen() failed!");
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
        throw;
    }

    std::string process_pid(std::to_string(getpid()));         // 현재 process의 PID를 받는다.
    std::string parent_process_pid(std::to_string(getppid())); // 현재 process의 parent process의 PID를 받는다.

    reply->set_process_info_reply(request->process_info_request() + process_pid + "\n" + request->parent_process_info_request() + parent_process_pid + "\n" + request->all_process_info_request() + "\n" + all_process_info);

    return Status::OK;
}