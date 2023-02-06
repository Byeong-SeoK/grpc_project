#include "../../include/moniter_server.h"
#include "../../include/save_log.h"

#include <glog/logging.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <typeinfo>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"
#include "time.h"

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_FATAL 10
#define LOG_LEVEL_ERROR 20
#define LOG_LEVEL_WARN 30
#define LOG_LEVEL_INFO 40
#define LOG_LEVEL_DEBUG 50
#define LOG_LEVEL_TRACE 60
#define LOG_LEVEL_ALL 100

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::ServiceLogReply;
using moniter::ServiceLogRequest;

using moniter::MoniterService;

// void MoniterServiceImpl::writeLog(std::string name, int line, int lv, const char *str, ...)
// {
//     time_t currentSec = time(NULL);
//     tm *t = localtime(&currentSec);
//     std::string strDataTime = std::to_string(t->tm_mon + 1) + "/" + std::to_string(t->tm_mday) + " " +
//                               std::to_string(t->tm_hour) + ":" + std::to_string(t->tm_min) + ":" + std::to_string(t->tm_sec);
//     std::cout << strDataTime << std::endl;

//     std::string DirLog = "../../../../file/"; // 별다른 값을 주지 않고 default로 냅두면 src/cmake/build에 저장된다.
//     std::string strLogFileName = DirLog + name + ".txt";

//     FILE *fp = fopen(strLogFileName.c_str(), "a");
//     if (fp == NULL)
//     {
//         puts("fail to open file pointer");
//         return;
//     }
//     char *result = NULL;
//     char level[10];
//     switch (lv)
//     {
//     case (LOG_LEVEL_FATAL):
//         strcpy(level, "[FATAL]");
//         break;
//     case (LOG_LEVEL_ERROR):
//         strcpy(level, "[ERROR]");
//         break;
//     case (LOG_LEVEL_WARN):
//         strcpy(level, "[WARN] ");
//         break;
//     case (LOG_LEVEL_INFO):
//         strcpy(level, "[INFO] ");
//         break;
//     case (LOG_LEVEL_DEBUG):
//         strcpy(level, "[DEBUG]");
//         break;
//     case (LOG_LEVEL_TRACE):
//         strcpy(level, "[TRACE]");
//         break;
//     }

//     result = (char *)malloc(sizeof(char) * (21 + strlen(name.c_str()) + strlen(str) + 30));
//     sprintf(result, "%s %s [%s:%d] : %s\n", level, strDataTime, name, line, str); // level strDataTime [name:line] : str 형식으로 값을 지정할 수 있다.
//     va_list args;                                                                 // args를 가변 인자의 Point를 저장하게 되는 Struct로 선언한다.
//     va_start(args, str);                                                          // args에 str의 값을 할당하는 것으로 초기화 한다.
//     vfprintf(fp, result, args);                                                   // fp가 가리키는 파일에 result가 가지고 있는 형식 값대로 args를 작성한다.
//     va_end(args);                                                                 // args가 가리키는 값(str)을 더 이상 가리키지 못하도록 만든다.
//     free(result);
//     // va_start(args, str);
//     // vprintf(result, args);
//     // va_end(args);
//     // if (result != NULL)
//     // {
//     //     free(result);
//     // }
//     if (fp != NULL)
//     {
//         fclose(fp);
//     }

//     // time_t currentSec = time(NULL);
//     // tm *t = localtime(&currentSec);
//     // std::string strDataTime = std::to_string(t->tm_year) + std::to_string(t->tm_mon + 1) + "/" + std::to_string(t->tm_mday) + " " +
//     //                           std::to_string(t->tm_hour) + ":" + std::to_string(t->tm_min) + ":" + std::to_string(t->tm_sec); // 현재 시간 정보 계산

//     // std::string strDate = std::to_string(t->tm_year) + std::to_string(t->tm_mon + 1) + std::to_string(t->tm_mday); // 로그 파일은 날짜가 변동될 때 새로 만들어지도록 한다.

//     // std::string DirLog = "../../../../file/";             // 별다른 값을 주지 않고 default로 냅두면 src/cmake/build에 저장된다.
//     // std::string LogFile = DirLog + strDate + "__log.txt"; // 날짜별로 log를 저장하는 파일

//     // FILE *timefp = fopen(LogFile.c_str(), "a"); // 로그를 기록할 txt파일을 열어서 로그를 작성하도록 한다.
//     // if (timefp == NULL)                         // timefp가 가리키는 파일이 에러로 인해 존재하지 않을 때
//     // {
//     //     puts("fail to open file pointer");
//     //     return;
//     // }

//     // char *result = NULL;
//     // char log_status[10];

//     // switch (level)
//     // {
//     // case (1):
//     //     strcpy(log_status, "INFO");
//     //     break;
//     // case (2):
//     //     strcpy(log_status, "WARNNING");
//     //     break;
//     // case (3):
//     //     strcpy(log_status, "ERROR");
//     //     break;
//     // case (4):
//     //     strcpy(log_status, "FATAL");
//     //     break;
//     // }

//     // result = (char *)malloc(sizeof(char) * (21 + strlen(LogFile.c_str()) + strlen(log) + 30)); // result에 작성한 로그를 넣기 위해 동적할당한다.
//     // sprintf(result, "[%s - %s] : %s\n", log_status, strDataTime, log);                         // [level : strDataTime] : log 형식으로 값을 지정할 수 있다.
//     // va_list args;                                                                              // args를 가변 인자의 Point를 저장하게 되는 Struct로 선언한다.
//     // va_start(args, log);                                                                       // args에 str의 값을 할당하는 것으로 초기화 한다.
//     // vfprintf(timefp, result, args);                                                            // fp가 가리키는 파일에 result가 가지고 있는 형식 값대로 args를 작성한다.
//     // va_end(args);                                                                              // args가 가리키는 값(str)을 더 이상 가리키지 못하도록 만든다.
//     // free(result);                                                                              // result가 가리키고 있던 값을 해제시킨다.

//     // if (timefp != NULL)
//     // {
//     //     fclose(timefp);
//     // }

//     return;
// }

Status MoniterServiceImpl::service_log_monitor_method(ServerContext *context, const ServiceLogRequest *request, ServiceLogReply *reply)
{
    // writeLog("test", 10, 40, "test log");

    reply->set_service_log_reply(request->service_log_request());
    return Status::OK;
}