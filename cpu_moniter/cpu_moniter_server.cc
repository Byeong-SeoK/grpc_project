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

#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>

#include "sys/sysinfo.h"
#include "sys/types.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "examples/protos/cpu.grpc.pb.h"
#else
#include "cpu.grpc.pb.h"
#endif

using cpu::CpuMoniterReply;
using cpu::CpuMoniterRequest;
using cpu::CpuMoniterService;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

struct stJiffies
{
  int user;
  int nice;
  int system;
  int idle;
};

// Logic and data behind the server's behavior.
class CpuMoniterServiceImpl final : public CpuMoniterService::Service
{
  Status current_cpu_usage_moniter_method(ServerContext *context,
                                          const CpuMoniterRequest *request,
                                          CpuMoniterReply *reply) override
  {
    FILE *pStat = NULL;
    char cpuID[6] = {0};

    stJiffies curJiffies;
    stJiffies prevJiffies;

    pStat = fopen("/proc/stat", "r");
    fscanf(pStat, "%s %d %d %d %d", cpuID, &curJiffies.user,
           &curJiffies.nice, &curJiffies.system, &curJiffies.idle);

    stJiffies diffJiffies;
    diffJiffies.user = curJiffies.user - prevJiffies.user;
    diffJiffies.nice = curJiffies.nice - prevJiffies.nice;
    diffJiffies.system = curJiffies.system - prevJiffies.system;
    diffJiffies.idle = curJiffies.idle - prevJiffies.idle;

    int totalJiffies = diffJiffies.user + diffJiffies.nice + diffJiffies.system + diffJiffies.idle;

    printf("Cpu usage : %.2f%%\n", 100.0f * (1.0 - (diffJiffies.idle / (double)totalJiffies)));

    prevJiffies = curJiffies;
    fclose(pStat);
    sleep(1);

    return Status::OK;
  }

  void Calculate_CPU_Usage()
};

void RunServer()
{
  std::string server_address("0.0.0.0:50051");
  CpuMoniterServiceImpl service;

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
