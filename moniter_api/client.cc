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
#include "./include/moniter_server.h"
#include "./include/moniter_client.h"

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

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

  while (true)
  {
    std::cout << "========= Choose monitering service that you want =========" << std::endl;
    std::cout << "1. Memory usage monitering service" << std::endl;
    std::cout << "2. CPU usage monitering service" << std::endl;
    std::cout << "3. Disk usage monitering service" << std::endl;
    std::cout << "4. Process monitering service" << std::endl;
    std::cout << "0. Exit the monitering" << std::endl;

    int number = -1;
    std::cout << "Input service number: ";
    std::cin >> number;
    std::cout << std::endl;

    if (number == 0)
    {
      std::cout << "Exit" << std::endl;
      break;
    }
    else if (number == 1)
    {
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
    }
    else if (number == 2)
    {
      CpuMoniterClient cpu_client(grpc::CreateChannel(
          target_str,
          grpc::InsecureChannelCredentials()));

      std::string sentence("Current usage of CPU volume: ");
      while (true)
      {
        std::string reply = cpu_client.current_cpu_usage_moniter_method(sentence);
        std::cout << reply << std::endl;
      }
    }
    else if (number == 3)
    {
      DiskMoniterClient disk_client(grpc::CreateChannel(
          target_str,
          grpc::InsecureChannelCredentials()));

      std::string totalDisk_prefix("Current total volume of disk: ");
      std::string usedDisk_prefix("Current usage volume of disk: ");
      std::string availDisk_prefix("Current available volume of disk: ");

      std::cout << "========= The current information of disk =========" << std::endl;
      std::string DiskMoniterReply = disk_client.current_disk_usage_moniter_method(totalDisk_prefix, usedDisk_prefix, availDisk_prefix);

      std::cout << DiskMoniterReply << std::endl;
    }
    else if (number == 4)
    {
      ProcessMoniterClient process_client(grpc::CreateChannel(
          target_str,
          grpc::InsecureChannelCredentials()));

      std::string proc_prefix("Current process PID: ");
      std::string pproc_prefix("Current parent process PID: ");
      std::string aproc_prefix("All current process info: ");

      std::cout << "========= The current information of process =========" << std::endl;
      std::string ProcessMoniterReply = process_client.current_process_moniter_method(proc_prefix, pproc_prefix, aproc_prefix);
      std::cout << ProcessMoniterReply << std::endl;

      SelectedProcessMoniterClient selected_client(grpc::CreateChannel(
          target_str,
          grpc::InsecureChannelCredentials()));

      std::cout << "Input PID: ";
      std::string PID_number;
      std::cin >> PID_number;
      std::cout << std::endl;

      std::string pid_pro_prefix("Selected PID process info: \n");
      std::string SelectedProcessMoniterReply = selected_client.selected_process_moniter_method(PID_number, pid_pro_prefix);
      std::cout << SelectedProcessMoniterReply << std::endl;
    }
    else
    {
      std::cout << "Wrong input" << std::endl;
    }
    std::cout << "\n"
              << std::endl;
  }

  return 0;
}