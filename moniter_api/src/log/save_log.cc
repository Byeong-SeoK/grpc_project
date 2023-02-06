#include "../../include/moniter_server.h"
#include "../../include/save_log.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <typeinfo>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"
#include "sys/stat.h"
#include "time.h"

using moniter::ServiceLogReply;
using moniter::ServiceLogRequest;

using moniter::MoniterService;

void SaveLog::save_level_Log(int level, const char *log)
// std::cout << google::INFO << " " << google::WARNING << " " << google::ERROR << " " << google::FATAL << " " << google::ERROR << std::endl;
// 위 코드를 통해서 INFO, WARNING, ERROR, FATAL 각각 0,1,2,3 이라는 고유 번호로 구분할 수 있음을 알 수 있다.
{
    fLB::FLAGS_logtostderr = 0;

    time_t currentSec = time(NULL);
    tm *t = localtime(&currentSec);
    std::string strDataTime = std::to_string(t->tm_year - 100) + "." + std::to_string(t->tm_mon + 1) + "." + std::to_string(t->tm_mday) + " " +
                              std::to_string(t->tm_hour) + ":" + std::to_string(t->tm_min) + ":" + std::to_string(t->tm_sec); // 현재 시간 정보 계산

    std::string strDate = std::to_string(t->tm_year - 100) + ":" + std::to_string(t->tm_mon + 1) + ":" +
                          std::to_string(t->tm_mday); // 로그 파일은 날짜가 변동될 때 새로 만들어지도록 한다.
    // tm_year는 1900년부터 count 되므로 2023이면 123이 되어 100을 빼야한다. tm_mon은 0부터 count 되므로 2월을 1월처럼 표현이 되어 1을 더해야한다.

    // std::cout << level << std::endl;
    // std::cout << strDataTime << std::endl;
    // std::cout << strDate << std::endl;

    std::string logDir = "../../../../file/" + strDate;
    mkdir(logDir.c_str(), 0755); // 날짜별 로그 폴더 생성
    FLAGS_log_dir = logDir;

    if (level == 0)
    {
        LOG(INFO) << log << std::endl;
    }
    else if (level == 1)
    {
        LOG(WARNING) << log << std::endl;
    }
    else if (level == 2)
    {
        LOG(ERROR) << log << std::endl;
    }
    else if (level == 3)
    {
        LOG(FATAL) << log << std::endl;
    }

    return;
}