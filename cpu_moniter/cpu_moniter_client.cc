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

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/cpu.grpc.pb.h"
#else
#include "cpu.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using cpu::CpuMoniterReply;
using cpu::CpuMoniterRequest;
using cpu::CpuMoniterService;

class CpuMoniterClient
{
public:
  CpuMoniterClient(std::shared_ptr<Channel> channel)
      : stub_(CpuMoniterService::NewStub(channel)) {}
  std::string current_cpu_usage_moniter_method(const std::string &sentence)
  {
    // Data we are sending to the server.
    CpuMoniterRequest request;
    request.set_client_order_of_cpu(sentence);

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
      return reply.information_of_cpu();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<CpuMoniterService::Stub> stub_;
};

int main(int argc, char **argv)
{
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  std::string target_str;
  std::string arg_str("--target");
  if (argc > 1)
  {
    std::string arg_val = argv[1];
    size_t start_pos = arg_val.find(arg_str);
    if (start_pos != std::string::npos)
    {
      start_pos += arg_str.size();
      if (arg_val[start_pos] == '=')
      {
        target_str = arg_val.substr(start_pos + 1);
      }
      else
      {
        std::cout << "The only correct argument syntax is --target="
                  << std::endl;
        return 0;
      }
    }
    else
    {
      std::cout << "The only acceptable argument is --target=" << std::endl;
      return 0;
    }
  }
  else
  {
    target_str = "localhost:50051";
  }
  CpuMoniterClient client(grpc::CreateChannel(
      target_str,
      grpc::InsecureChannelCredentials())); // MemoryMoniter type class 생성

  std::string sentence("Current usage of CPU volume: ");
  while (true)
  {
    std::string reply = client.current_cpu_usage_moniter_method(sentence);
    std::cout << reply << std::endl;
  }

  return 0;
}