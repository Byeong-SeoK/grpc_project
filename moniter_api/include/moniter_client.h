#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using moniter::MemoryReply;
using moniter::MemoryRequest;

using moniter::CpuMoniterReply;
using moniter::CpuMoniterRequest;

using moniter::DiskMoniterReply;
using moniter::DiskMoniterRequest;

using moniter::ProcessMoniterReply;
using moniter::ProcessMoniterRequest;

using moniter::MoniterService;

class MemoryClient
{
public:
    MemoryClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string current_memory_moniter_method(const std::string virtual_memory_request,
                                              const std::string physical_memory_request,
                                              const std::string avail_virtual_memory_request,
                                              const std::string avail_physical_memory_request)
    {
        // Data we are sending to the server.
        MemoryRequest request;
        request.set_virtual_memory_request(virtual_memory_request);
        request.set_physical_memory_request(physical_memory_request);
        request.set_avail_virtual_memory_request(avail_virtual_memory_request);
        request.set_avail_physical_memory_request(avail_physical_memory_request);

        // Container for the data we expect from the server.
        MemoryReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status =
            stub_->current_memory_moniter_method(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            return reply.memory_info_reply();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};

class CpuMoniterClient
{
public:
    CpuMoniterClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}
    std::string current_cpu_usage_moniter_method(const std::string &sentence)
    {
        // Data we are sending to the server.
        CpuMoniterRequest request;
        request.set_cpu_request(sentence);

        // Container for the data we expect from the server.
        CpuMoniterReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status =
            stub_->current_cpu_usage_moniter_method(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            return reply.cpu_reply();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};

class DiskMoniterClient
{
public:
    DiskMoniterClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}
    std::string current_disk_usage_moniter_method(const std::string &total_disk_volume_request,
                                                  const std::string &disk_usage_request,
                                                  const std::string &avail_disk_volume_request)
    {
        // Data we are sending to the server.
        DiskMoniterRequest request;
        request.set_total_disk_volume_request(total_disk_volume_request);
        request.set_disk_usage_request(disk_usage_request);
        request.set_avail_disk_volume_request(avail_disk_volume_request);

        // Container for the data we expect from the server.
        DiskMoniterReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status =
            stub_->current_disk_usage_moniter_method(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            return reply.disk_info_reply();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};

class ProcessMoniterClient
{
public:
    ProcessMoniterClient(std::shared_ptr<Channel> channel)
        : stub_(MoniterService::NewStub(channel)) {}
    std::string current_process_moniter_method(const std::string &process_info_request,
                                               const std::string &parent_process_info_request,
                                               const std::string &all_process_info_request)
    {
        ProcessMoniterRequest request;
        request.set_process_info_request(process_info_request);
        request.set_parent_process_info_request(parent_process_info_request);
        request.set_all_process_info_request(all_process_info_request);

        ProcessMoniterReply reply;

        ClientContext context;

        // The actual RPC.
        Status status =
            stub_->current_process_moniter_method(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            return reply.process_info_reply();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<MoniterService::Stub> stub_;
};