#include "../../include/moniter_client.h"

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

using moniter::SelectedProcessMoniterReply;
using moniter::SelectedProcessMoniterRequest;

using moniter::MoniterService;

std::string MoniterClient::selected_process_moniter_method(const std::string &selected_process_name,
                                                           const std::string &pid_process_info_request)
{
    SelectedProcessMoniterRequest request;
    request.set_pid_process_info_request(pid_process_info_request);
    request.set_selected_process_name(selected_process_name);

    SelectedProcessMoniterReply reply;

    ClientContext context;

    // The actual RPC.
    Status status =
        stub_->selected_process_moniter_method(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
        LOG(INFO) << "Selected process monitoring service API Success" << std::endl;
        return reply.selected_process_info_reply();
    }
    else
    {
        LOG(ERROR) << status.error_code() << ": " << status.error_message() << std::endl;
        // std::cout << status.error_code() << ": " << status.error_message()
        //           << std::endl;
        return "RPC failed";
    }
}