#include "../../include/moniter_server.h"
#include "../../include/set_log_dir.h"

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"
#include "sys/stat.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::CpuMoniterReply;
using moniter::CpuMoniterRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::current_cpu_usage_moniter_method(ServerContext *context,
                                                            const CpuMoniterRequest *request,
                                                            CpuMoniterReply *reply)
{
    SetlogDir dir;
    std::string logDir = dir.setDir();

    FLAGS_log_dir = logDir;
    mkdir(logDir.c_str(), 0755); // 여기까지가 로그가 저장될 파일 경로 만들기

    LOG(INFO) << "cpu monitoring API start .";

    FILE *pStat = NULL;
    char cpuID[6] = {0};

    pStat = fopen("/proc/stat", "r"); // proc/stat을 read 모드로 열어서 cpu 상태를 읽어와 pStat에 저장한다.
    if (pStat == nullptr)
    {
        LOG(ERROR) << "cpu monitoring API - Error";
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Cannot find /proc/stat");
    }

    fscanf(pStat, "%s %d %d %d %d", cpuID, &curJiffies.user, &curJiffies.nice,
           &curJiffies.system, &curJiffies.idle);

    stJiffies diffJiffies;                                       // 리눅스에서는 내부적으로 jiffy라는 단위 시간을 사용한다. 결국 이 수치 값은 부팅 후 지금까지 소모된 jiffies의 #이다.
    diffJiffies.user = curJiffies.user - prevJiffies.user;       // user :사용자모드에서 CPU가 소비된 시간의 비율
    diffJiffies.nice = curJiffies.nice - prevJiffies.nice;       // nice: nice로 스케줄링의 우선도를 변경한 프로세스가 사용자 모드에서 CPU를 소비한 시간의 비율
    diffJiffies.system = curJiffies.system - prevJiffies.system; // system: 시스템 모드에서 CPU가 소비된 시간의 비율
    diffJiffies.idle = curJiffies.idle - prevJiffies.idle;       // CPU가 디스크I/O 대기등으로 대기되지 않고, Idle상태로 소비한 시간의 비율

    int totalJiffies = diffJiffies.user + diffJiffies.nice +
                       diffJiffies.system + diffJiffies.idle; // cpu 전체 크기

    diffJiffies.usage = 100.0f * (1.0 - (diffJiffies.idle / (double)totalJiffies)); // 현재 사용 중인 cpu 크기

    prevJiffies = curJiffies; // 시간의 비율로 cpu 이용량을 계산하므로 이전 시간의 비욜울 현재 측정한 시간의 비율로 바꿔야 실시간 측정이 가능하다.
    fclose(pStat);
    sleep(1);

    reply->set_cpu_reply(request->cpu_request() + std::to_string(diffJiffies.usage) + '%');

    LOG(INFO) << "cpu monitoring API end .";

    return Status::OK;
}