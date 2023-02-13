#include "../../include/moniter_client.h"
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <typeinfo>

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

void MoniterClient::readLog()
{

    time_t currentSec = time(NULL);
    tm *t = localtime(&currentSec);
    std::string strDate = std::to_string(t->tm_year - 100) + "." + std::to_string(t->tm_mon + 1) + "." +
                          std::to_string(t->tm_mday); // 로그 파일은 날짜가 변동될 때 새로 만들어지도록 한다.
    // tm_year는 1900년부터 count 되므로 2023이면 123이 되어 100을 빼야한다. tm_mon은 0부터 count 되므로 2월을 1월처럼 표현이 되어 1을 더해야한다.

    google::LogSink::send();
}