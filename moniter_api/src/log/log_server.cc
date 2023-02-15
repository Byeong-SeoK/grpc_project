#include "../../include/moniter_server.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <typeinfo>
#include <unistd.h>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"
#include "sys/stat.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::LogReply;
using moniter::LogRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::server_log_monitor_method(ServerContext *context,
                                                     const LogRequest *request,
                                                     LogReply *reply)
{
    std::string LogDate = request->log_date_request();
    std::string comType = request->log_com_request();
    std::string LogType = request->log_type_request();
    std::string reply_message;

    std::string dirPath = "/mnt/c/Users/INNO-C-535/grpc_project/file/" + LogDate + "/";
    std::string logLocation = comType + ".DESKTOP-8L601US.byeongseok.log." + LogType + ".";
    std::regex re("\.(\d*-\d*.\d*)");

    std::vector<std::string> fileVec;
    char filebuffer[1024];
    chdir(dirPath.c_str()); // 사용자가 입력한 날짜의 로그 폴더로 이동

    FILE *pipe = popen("ls", "r"); // ls 명령어로 조회할 수 있는 정보를 읽어와서 pipe에 저장한다.
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    try
    {
        while (fgets(filebuffer, sizeof(filebuffer), pipe) != NULL)
        {
            fileVec.push_back(filebuffer); // ls를 통해 검색할 수 있는 모든 로그 파일을 fileVec 벡터에 저장한다.
        }
    }
    catch (...)
    {
        pclose(pipe);
        return grpc::Status(grpc::StatusCode::DEADLINE_EXCEEDED, "popen() failed!");
    }

    chdir("/mnt/c/Users/INNO-C-535/grpc_project/moniter_api/src/cmake/build");
    for (int i = 0; i < fileVec.size(); i++)
    {
        std::vector<std::string> lines;
        std::smatch match;

        if (std::regex_search(fileVec[i], match, re)) // 해당 정규표현식 패턴을 만족하는 파일만 열어서 읽는다.
        {
            std::string line;

            std::string extension = fileVec[i].substr(43);
            std::string path = (dirPath + logLocation + extension).c_str();

            path.erase(std::remove_if(path.begin(), path.end(), isspace), path.end()); // path는 string이므로 제일 뒤에 붙는 "\n" 이라는 개행문자를 지워야한다.

            std::ifstream ifs;
            ifs.open(path.c_str());
            if (!ifs.is_open())
            {
                continue;
                // std::cerr << "Error: " << strerror(errno) << std::endl; // 어떤 에러가 발생하였는지 확인
            }

            while (getline(ifs, line))
            {
                lines.push_back(line);
            }

            // for (const auto i : lines)
            //     std::cout << i << std::endl;

            std::cout << std::endl;
            ifs.close();

            std::ofstream ofs;
            std::string log_monitor_path = "/mnt/c/Users/INNO-C-535/grpc_project/log_data/";

            chdir(log_monitor_path.c_str());
            std::string filename = LogDate + "." + comType + "." + LogType + ".txt";
            reply_message = filename;
            filename.erase(std::remove_if(filename.begin(), filename.end(), isspace), filename.end()); // path는 string이므로 제일 뒤에 붙는 "\n" 이라는 개행문자를 지워야한다.

            ofs.open(filename);
            if (ofs.is_open())
            {
                for (const auto i : lines)
                {
                    ofs << i << std::endl;
                }
            }
        }
    }
    reply->set_log_reply("You can access log data by this file: " + reply_message);

    return Status::OK;
}