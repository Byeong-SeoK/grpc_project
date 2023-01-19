/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include "sys/sysinfo.h"
#include "sys/types.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "examples/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using moniter::MemoryMoniterService;
using moniter::MemoryReply;
using moniter::MemoryRequest;

// Logic and data behind the server's behavior.
class MemoryMoniterServiceImpl final : public MemoryMoniterService::Service
{
  Status test_method(ServerContext *context, const MemoryRequest *request,
                     MemoryReply *reply) override
  {
    std::string prefix("Test monitoring server: ");
    reply->set_message(prefix + request->name());

    return Status::OK;
  }

  Status current_memory_moniter_method(ServerContext *context,
                                       const MemoryRequest *request,
                                       MemoryReply *reply) override
  {
    struct sysinfo memInfo;

    sysinfo(&memInfo);
    long long virtualMemUsed =
        memInfo.totalswap -
        memInfo.freeswap; // 지금 사용하고 있는 virtual memory volume
    virtualMemUsed *= memInfo.mem_unit;

    // meminfo.mem_unit의 값은 1로 1byte를 의미한다.

    long long physMemUsed =
        memInfo.totalram -
        memInfo.freeram; // 지금 사용하고 있는 physcal memory volume
    physMemUsed *= memInfo.mem_unit;

    long long availVirMem = 0; // 현재 가용할 수 있는 가상 메모리 용량
    long long availPhyMem = 0; // 현재 가용할 수 있는 물리적 메모리 용량

    virtualMemUsed = virtualMemUsed / (1024 * 1024);
    physMemUsed = physMemUsed / (1024 * 1024);
    availVirMem = memInfo.freeswap / (1024 * 1024);
    availPhyMem = memInfo.freeram / (1024 * 1024);
    // 이때 virtualMemUsed, physMemUsed, availVirMem, availPhymem 값 모두 byte 단위이다.
    // 사용자가 알아보기 편하게 1024로 나누어서 MB로 표현하도록 한다.

    std::string v_suffix(std::to_string(virtualMemUsed)); // 현재 사용중인 가상 메모리 용량을 받는 변수
    std::string p_suffix(std::to_string(physMemUsed));    // 현재 사용중인 물리적 메모리 용량을 받는 변수
    std::string avm_suffix(std::to_string(availVirMem));  // 현재 가용할 수 있는 가상 메모리 용량을 받는 변수
    std::string apm_suffix(std::to_string(availPhyMem));  // 현재 가용할 수 있는 물리적 메모리 용량을 받는 변수
    reply->set_message(
        request->+ v_suffix + "MB" +
        "\nCurrent usage of physical memory volume: " + p_suffix + "MB");

    return Status::OK;
  }
};

void RunServer()
{
  std::string server_address("0.0.0.0:50051");
  MemoryMoniterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv)
{
  RunServer();

  return 0;
}
