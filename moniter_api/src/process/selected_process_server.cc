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

using moniter::SelectedProcessMoniterReply;
using moniter::SelectedProcessMoniterRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::selected_process_moniter_method(ServerContext *context,
                                                           const SelectedProcessMoniterRequest *request,
                                                           SelectedProcessMoniterReply *reply)
{
    char pid_buffer[128];
    std::string pid_process_info = "";
    std::string temp = "ps -ef | grep " + request->selected_process_name(); // request
    const char *command = temp.c_str();
    FILE *pipe2 = popen(command, "r");
    if (!pipe2)
        throw std::runtime_error("popen() failed!");
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
        throw;
    }

    reply->set_selected_process_info_reply(request->pid_process_info_request() + pid_process_info);

    return Status::OK;
}