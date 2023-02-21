#include "../../include/moniter_server.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <typeinfo>
#include <unistd.h>
#include <experimental/filesystem>

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

namespace fs = std::experimental::filesystem::v1;

Status MoniterServiceImpl::server_log_monitor_method(ServerContext *context,
                                                     const LogRequest *request,
                                                     LogReply *reply)
{
    LOG(INFO) << "Log monitoring service API start .";

    std::string LogDate = request->log_date_request();
    std::string comType = request->log_com_request();
    std::string LogType = request->log_type_request();

    std::string lines; // 로그 파일 데이터 전체를 받는 변수
    std::string line;  // 로그 파일 한 줄씩 읽어올 때 받는 변수

    fs::path p(("../../../../file/" + LogDate).c_str());
    for (auto &path : fs::recursive_directory_iterator(p.c_str()))
    {
        std::ifstream ifs;
        if (comType == "client" && path.path().c_str()[24] == 's')
        {
            continue;
        }
        else if (comType == "server" && path.path().c_str()[24] == 'c')
        {
            continue;
        }
        else
        {
            ifs.open(path.path().c_str());
            if (path.path().string().size() < 40) //.symlink 확장자 파일은 건너뛰게 만든다.
            {
                LOG(INFO) << ".symlink extension file do not have to open";
                continue;
            }

            if (!ifs.is_open())
            {
                std::cout << path << std::endl;
                LOG(ERROR) << "file opened failed!!";
                std::cerr << "Error: " << strerror(errno) << std::endl; // 어떤 에러가 발생하였는지 확인
                continue;
            }

            while (getline(ifs, line)) // 파일을 처음부터 끝까지 한줄 씩 읽어서 line에 저장한다.
            {
                lines = lines + line + "\n";
            }
        }
    }

    LOG(INFO) << "Log monitoring service API end .";
    reply->set_log_reply(lines);
    return Status::OK;
}

// Below this code is the file input & ouput code to read & write log by using regular expression

// Status MoniterServiceImpl::server_log_monitor_method(ServerContext *context,
//                                                      const LogRequest *request,
//                                                      LogReply *reply)
// {
//     std::string LogDate = request->log_date_request();
//     std::string comType = request->log_com_request();
//     std::string LogType = request->log_type_request();
//     std::string reply_message;

//     std::string log_server_dirPath = "/mnt/c/Users/INNO-C-535/grpc_project/file/" + LogDate + "/";
//     std::string logLocation = comType + ".DESKTOP-8L601US.byeongseok.log." + LogType;
//     std::regex re("\\.(\\d*-\\d*.\\d*)"); // 로그 파일의 확장자만 추출해내는 정규표현식

//     std::vector<std::string> fileVec;
//     char filebuffer[1024];
//     chdir(log_server_dirPath.c_str()); // 사용자가 입력한 날짜의 로그 폴더로 이동

//     FILE *pipe = popen("ls", "r"); // ls 명령어로 조회할 수 있는 정보를 읽어와서 pipe에 저장한다.
//     if (!pipe)
//     {
//         LOG(ERROR) << "popen() failed!";
//         throw std::runtime_error("popen() failed!");
//     }
//     try
//     {
//         while (fgets(filebuffer, sizeof(filebuffer), pipe) != NULL)
//         {
//             fileVec.push_back(filebuffer); // ls를 통해 검색할 수 있는 모든 로그 파일을 fileVec 벡터에 저장한다.
//         }
//     }
//     catch (...)
//     {
//         pclose(pipe);
//         return grpc::Status(grpc::StatusCode::DEADLINE_EXCEEDED, "popen() failed!");
//     }

//     for (int i = 0; i < fileVec.size(); i++)
//     {
//         std::vector<std::string> lines;
//         std::smatch match; // fileVec[i]에서 정규표현식에 매치되는 부분 문자열을 담는다.

//         if (std::regex_search(fileVec[i], match, re)) // 해당 정규표현식 패턴을 만족하는 파일만 열어서 읽기 위해 패턴을 찾는다.
//         {
//             // std::cout << match.str() << std::endl;
//             std::string line;

//             std::string path = (log_server_dirPath + logLocation + match[2].str()).c_str();

//             path.erase(std::remove_if(path.begin(), path.end(), isspace), path.end()); // path는 string이므로 제일 뒤에 붙는 "\n" 이라는 개행문자를 지워야한다.

//             std::ifstream ifs;
//             ifs.open(path.c_str());
//             if (!ifs.is_open())
//             {
//                 // std::cout << path << std::endl;
//                 // LOG(ERROR) << "file opened failed!!";
//                 // std::cerr << "Error: " << strerror(errno) << std::endl; // 어떤 에러가 발생하였는지 확인
//                 continue;
//             }

//             while (getline(ifs, line)) // 파일을 처음부터 끝까지 한줄 씩 읽어서 line에 저장한다.
//             {
//                 lines.push_back(line);
//             }

//             ifs.close();

//             std::ofstream ofs;
//             std::string log_client_dirPath = "/mnt/c/Users/INNO-C-535/grpc_project/log_data/";

//             chdir(log_client_dirPath.c_str()); // 로그를 저장할 폴더로 디렉토리를 변경한다.
//             std::string filename = LogDate + "." + comType + "." + LogType + ".txt";
//             reply_message = filename;
//             filename.erase(std::remove_if(filename.begin(), filename.end(), isspace), filename.end()); // path는 string이므로 제일 뒤에 붙는 "\n" 이라는 개행문자를 지워야한다.

//             ofs.open(filename, std::ios::app); // std::ios::app을 통해서 기존에 있던 파일에 내용을 추가하도록 만든다.
//             if (!ofs.is_open())
//             {
//                 LOG(ERROR) << "file opened failed!!!";
//                 std::cerr << "Error: " << strerror(errno) << std::endl; // 어떤 에러가 발생하였는지 확인
//                 continue;
//             }

//             for (const auto i : lines)
//             {
//                 ofs << i << std::endl;
//             }
//             ofs << std::endl;
//         }
//     }
//     reply->set_log_reply("You can access log data by this file: /grpc_project/log_data/" + reply_message);

//     return Status::OK;
// }