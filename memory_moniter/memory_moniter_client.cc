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
#include "examples/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using moniter::MemoryReply;
using moniter::MemoryRequest;
using moniter::MoniterService;

// Setting value of the moniter proto's memory object variables
// Memory MakeMemInfo(const std::string virtual_memory_prefix,
//                    const std::string physical_memory_prefix,
//                    const std::string avail_virtual_memory_prefix,
//                    const std::string avail_physical_memory_prefix)
// {
//   Memory m;
//   m.set_virtual_memory_request(virtual_memory_prefix);
//   m.set_physical_memory_request(physical_memory_prefix);
//   m.set_avail_virtual_memory_request(avail_virtual_memory_prefix);
//   m.set_avail_physical_memory_request(avail_physical_memory_prefix);

//   return m;
// }

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
  MemoryClient memory_client(grpc::CreateChannel(
      target_str,
      grpc::InsecureChannelCredentials()));                                                  // MemoryMoniter type class 생성
                                                                                             // client order of memory object define
  std::string virtual_memory_prefix("Current usage of virtual memory volume: ");             // 현재 사용 중인 가상 메모리 용량
  std::string physical_memory_prefix("Current usage of physical memeory volume: ");          // 현재 사용 중인 물리적 메모리 용량
  std::string avail_virtual_memory_prefix("Current available of virtual memory volume: ");   // 현재 가용한 가상 메모리 용량
  std::string avail_physical_memory_prefix("Current available of physical memory volume: "); // 현재 가용한 물리적 메모리 용량

  std::cout << "========= The current usage of memory system =========" << std::endl;
  std::string memory_reply = memory_client.current_memory_moniter_method(virtual_memory_prefix,
                                                                         physical_memory_prefix,
                                                                         avail_virtual_memory_prefix,
                                                                         avail_physical_memory_prefix);
  std::cout << memory_reply << std::endl;

  return 0;
}