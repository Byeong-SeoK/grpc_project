#include "../../include/moniter_client.h"
#include <grpcpp/grpcpp.h>

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

// #ifdef LOG
// #undef LOG
// #define LOG(severity) EZ_COMPACT_GOOGLE_LOG_##severity.stream()
// #else

// #define EZ_COMPACT_GOOGLE_LOG_INFO EzLogMessage(__FILE__, __LINE__, google::GLOG_INFO)
// #define EZ_COMPACT_GOOGLE_LOG_ERROR EzLogMessage(__FILE__, __LINE__, google::GLOG_ERROR)
// #define EZ_COMPACT_GOOGLE_LOG_WARNING EzLogMessage(__FILE__, __LINE__, google::GLOG_WARNING)

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

using moniter::MoniterService;

void MoniterClient::readLog(std::string LogDate, std::string comType, std::string LogType)
{
    struct stat st;

    std::string dirPath = "/mnt/c/Users/INNO-C-535/grpc_project/file/" + LogDate + "/";
    // std::string logLocation = comType + ".DESKTOP-8L601US.byeongseok.log." + LogType + ".";
    std::regex re("\d*-\d*\.\d*");

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
            // std::cout << fgets(filebuffer, sizeof(filebuffer), pipe) << std::endl;
            fileVec.push_back(filebuffer); // ls를 통해 검색할 수 있는 모든 로그 파일을 fileVec 벡터에 저장한다.
        }
    }
    catch (...)
    {
        pclose(pipe);
        return;
    }

    chdir("/mnt/c/Users/INNO-C-535/grpc_project/moniter_api/src/cmake/build");
    for (int i = 0; i < fileVec.size(); i++)
    {
        std::vector<std::string> lines;
        std::smatch match;
        // std::cout << std::regex_search(fileVec[i], match, re) << std::endl;

        if (std::regex_search(fileVec[i], match, re))
        {
            std::string line;

            std::string path = (dirPath + fileVec[i]).c_str();
            // std::string path = "../../../../file/" + LogDate + "/" + fileVec[i];
            std::ifstream ifs;
            ifs.open(path.c_str());
            if (!ifs.is_open())
            {
                std::cerr << "Error: " << strerror(errno) << std::endl; // 에러 확인
                return;
            }
            std::cout << "fine" << std::endl;
            while (getline(ifs, line))
            {
                lines.push_back(line);
            }

            for (const auto i : lines)
                std::cout << i << std::endl;

            ifs.close();
        }
    }

    return;
}
