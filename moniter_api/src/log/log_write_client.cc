#include "../../include/moniter_client.h"

#include "sys/stat.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <typeinfo>
#include <unistd.h>
#include <experimental/filesystem>
#include <glog/logging.h>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using moniter::LogReply;
using moniter::LogRequest;

using moniter::MoniterService;

std::string MoniterClient::client_log_write_method(const std::string &LogDate, const std::string &comType,
                                                   const std::string &LogType, std::string received_log_data)
{
    std::ofstream ofs;
    std::string dirpath = "/mnt/c/Users/INNO-C-535/grpc_project/log_data/";

    std::string filename = dirpath + LogDate + "." + comType + "." + LogType + ".txt";
    filename.erase(std::remove_if(filename.begin(), filename.end(), isspace), filename.end());
    // path는 string이므로 제일 뒤에 붙는 "\n" 이라는 개행문자를 지워야한다.

    ofs.open(filename, std::ios::app); // std::ios::app을 통해서 기존에 있던 파일에 내용을 추가하도록 만든다.
    if (!ofs.is_open())
    {
        LOG(ERROR) << "file opened failed!!!";
        std::cerr << "Error: " << strerror(errno) << std::endl; // 어떤 에러가 발생하였는지 확인
    }

    for (int i = 0; i < received_log_data.size(); i++)
    {
        if (received_log_data[i] == '\n')
        {
            ofs << std::endl;
        }
        else if (received_log_data[i] == '.' && received_log_data[i + 1] == '\n')
        {
            ofs << received_log_data[i] << '\n'
                << std::endl;
        }
        else
        {
            ofs << received_log_data[i];
        }
    }
    ofs << std::endl;

    ofs.close();

    LOG(INFO) << "Log monitoring service API Success .";
    std::string reply_message = "You can access log data by this file: " + filename;
    return reply_message;
}