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
#include <glog/logging.h>

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "../include/moniter_server.h"
#include "../include/moniter_client.h"
#include "../include/client_UI.h"

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

  google::InitGoogleLogging(argv[0]);
  MoniterClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())); // client 객체 생성 및 gRPC 통신 채널 생성
  Client_UI UI;                                                                              // client UI를 다루는 객체 생성

  std::cout << std::endl;
  while (true)
  {
    UI.show_menu();

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
      UI.set_request(1);

      std::cout << "========= The current usage of memory system =========" << std::endl;

      std::string memory_reply = client.current_memory_moniter_method(UI.get_request()[0], UI.get_request()[1], UI.get_request()[2], UI.get_request()[3]);
      std::cout << memory_reply << std::endl;
    }
    else if (number == 2)
    {
      UI.set_request(2);
      while (true)
      {
        std::string reply = client.current_cpu_usage_moniter_method(UI.get_request()[0]);
        std::cout << reply << std::endl;
      }
    }
    else if (number == 3)
    {
      UI.set_request(3);

      std::cout << "========= The current information of disk =========" << std::endl;
      std::string DiskMoniterReply = client.current_disk_usage_moniter_method(UI.get_request()[0], UI.get_request()[1], UI.get_request()[2]);
      std::cout << DiskMoniterReply << std::endl;
    }
    else if (number == 4)
    {
      UI.set_request(4);

      std::cout << "========= The current information of process =========" << std::endl;
      std::string ProcessMoniterReply = client.current_process_moniter_method(UI.get_request()[0], UI.get_request()[1], UI.get_request()[2]);
      std::cout << ProcessMoniterReply << std::endl;

      std::cout << "Input Process name that you want to monitor: ";
      std::string selected_process_name;
      std::cin >> selected_process_name;
      std::cout << std::endl;

      std::string pid_pro_prefix("Selected PID process info: \n"); // Process information that user selected
      std::string SelectedProcessMoniterReply = client.selected_process_moniter_method(selected_process_name, pid_pro_prefix);
      std::cout << SelectedProcessMoniterReply << std::endl;
    }
    else if (number == 5)
    {
      std::string log_request_prefix("Log: ");
      while (true)
      {
        std::string log_service_reply = client.service_log_monitor_method(log_request_prefix);
        std::cout << log_service_reply << std::endl;
      }
    }
    else
    {
      std::cout << "Wrong input" << std::endl;
    }

    UI.clear_request_vector();
    std::cout << "\n"
              << std::endl;
  }

  return 0;
}