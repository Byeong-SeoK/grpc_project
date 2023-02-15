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

#include <sys/stat.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "../include/moniter_server.h"
#include "../include/moniter_client.h"
#include "../include/client_UI.h"
#include "../include/set_log_dir.h"

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

int main(int argc, char **argv)
{
  fLB::FLAGS_logtostderr = 0;
  FLAGS_log_dir = "../../../../file/";

  google::InitGoogleLogging(argv[0]); //./client에 대한 것들에 대해 log를 기록

  Client_UI UI; // client UI를 다루는 객체 생성

  SetlogDir dir;
  std::string logDir = dir.setDir();
  mkdir(logDir.c_str(), 0755); // 날짜별 로그 폴더 생성, 폴더 접근 권한은 0755가 가장 기본적인 값이다.
  FLAGS_log_dir = logDir;      // client log가 저장될 파일 경로 지정 및 날짜별 폴더 생성
  // google::SetLogFilenameExtension(".txt");

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
        LOG(ERROR) << "The only correct argument syntax is --target=";
        google::FlushLogFiles(google::GLOG_ERROR);
        return 0;
      }
    }
    else
    {
      LOG(ERROR) << "The only acceptable argument is --target=";
      google::FlushLogFiles(google::GLOG_ERROR);
      return 0;
    }
  }
  else
  {
    target_str = "localhost:50051";
  }

  MoniterClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())); // client 객체 생성 및 gRPC 통신 채널 생성
  std::cout << std::endl;

  while (true)
  {
    UI.show_menu();

    int number = -1;
    std::cout << "Input service number: ";
    std::cin >> number;
    std::cout << std::endl;

    // below two lines of code is for test
    // int number = rand() % 4 + 1;
    // std::cout << "This is for test: ";

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
      int repeat = 0; // cpu API 실행 반복 횟수
      while (repeat < 4)
      {
        std::string cpu_reply = client.current_cpu_usage_moniter_method(UI.get_request()[0]);
        std::cout << cpu_reply << std::endl;
        repeat = repeat + 1;
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

      std::string SelectedProcessMoniterReply = client.selected_process_moniter_method(selected_process_name, UI.get_request()[3]);
      std::cout << SelectedProcessMoniterReply << std::endl;
    }
    else if (number == 5)
    {
      std::cout << "========= Log inquire service =========" << std::endl;
      std::string LogDate;
      std::string comType;
      std::string LogType;

      std::cout << "Input log record date that you want to inquire: ";
      std::cin >> LogDate;
      std::cout << "Input computer type where the log is record: ";
      std::cin >> comType;
      std::cout << "Input log type that you want to inquire: ";
      std::cin >> LogType;
      std::cout << std::endl;
      std::string LogMonitorReply = client.log_monitor_method(LogDate, comType, LogType);
      std::cout << LogMonitorReply << std::endl;
      // client.readLog(LogDate, comType, LogType);
    }
    else
    {
      std::cout << "Wrong input" << std::endl;
      LOG(WARNING) << "Wrong input service number";
      google::FlushLogFiles(google::GLOG_WARNING);
    }

    UI.clear_request_vector();
    std::cout << "\n"
              << std::endl;
  }

  LOG(INFO) << "Client terminated .";

  google::FlushLogFiles(google::GLOG_INFO);

  return 0;
}