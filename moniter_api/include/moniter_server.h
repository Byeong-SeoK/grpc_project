#include <iostream>
#include <memory>
#include <string>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "./fileapi.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "examples/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using moniter::MemoryReply;
using moniter::MemoryRequest;

using moniter::CpuMoniterReply;
using moniter::CpuMoniterRequest;

using moniter::DiskMoniterReply;
using moniter::DiskMoniterRequest;

using moniter::MoniterService;

struct stJiffies // 실시간 cpu 사용량 측정을 위한 구조체
{
    int user;
    int nice;
    int system;
    int idle;
    double usage = 0.0;
};
stJiffies curJiffies;
stJiffies prevJiffies;

// typedef struct _drive_info // disk 이용율 측정을 위한 구조체
// {
//     unsigned __int64 usedBytes;
//     unsigned __int64 totoalFreeBytes;
//     unsigned __int64 freeBytes;
// } DRIVE_INFO;

// Logic and data behind the server's behavior.
class MoniterServiceImpl final : public MoniterService::Service
{
    Status current_memory_moniter_method(ServerContext *context,
                                         const MemoryRequest *request,
                                         MemoryReply *reply) override
    {
        struct sysinfo memInfo;

        sysinfo(&memInfo);
        long long virtualMemUsed =
            memInfo.totalswap -
            memInfo.freeswap; // 지금 사용하고 있는 virtual memory volume
        virtualMemUsed *= memInfo.mem_unit;

        // meminfo.mem_unit의 값은 1로 1byte를 의미한다.

        long long physMemUsed =
            memInfo.totalram -
            memInfo.freeram; // 지금 사용하고 있는 physcal memory volume
        physMemUsed *= memInfo.mem_unit;

        long long availVirMem = 0; // 현재 가용할 수 있는 가상 메모리 용량
        long long availPhyMem = 0; // 현재 가용할 수 있는 물리적 메모리 용량

        virtualMemUsed = virtualMemUsed / (1024 * 1024);
        physMemUsed = physMemUsed / (1024 * 1024);
        availVirMem = memInfo.freeswap / (1024 * 1024);
        availPhyMem = memInfo.freeram / (1024 * 1024);
        // 이때 virtualMemUsed, physMemUsed, availVirMem, availPhymem 값 모두 byte 단위이다.
        // 사용자가 알아보기 편하게 1024로 나누어서 MB로 표현하도록 한다.

        std::string v_suffix(std::to_string(virtualMemUsed)); // 현재 사용중인 가상 메모리 용량을 받는 변수
        std::string p_suffix(std::to_string(physMemUsed));    // 현재 사용중인 물리적 메모리 용량을 받는 변수
        std::string avm_suffix(std::to_string(availVirMem));  // 현재 가용할 수 있는 가상 메모리 용량을 받는 변수
        std::string apm_suffix(std::to_string(availPhyMem));  // 현재 가용할 수 있는 물리적 메모리 용량을 받는 변수

        reply->set_memory_info_reply(request->virtual_memory_request() + v_suffix + "MB\n" + request->physical_memory_request() + p_suffix + "MB\n" + request->avail_virtual_memory_request() + avm_suffix + "MB\n" + request->avail_physical_memory_request() + apm_suffix + "MB");
        // 이 set_*()의 인자는 request->* 하나만 받는다. 그래서 여러 개의 request->*를 넘기면 build 오류가 발생한다.
        return Status::OK;
    }

    Status current_cpu_usage_moniter_method(ServerContext *context,
                                            const CpuMoniterRequest *request,
                                            CpuMoniterReply *reply) override
    {
        FILE *pStat = NULL;
        char cpuID[6] = {0};

        pStat = fopen("/proc/stat", "r");
        fscanf(pStat, "%s %d %d %d %d", cpuID, &curJiffies.user, &curJiffies.nice,
               &curJiffies.system, &curJiffies.idle);

        stJiffies diffJiffies;
        diffJiffies.user = curJiffies.user - prevJiffies.user;
        diffJiffies.nice = curJiffies.nice - prevJiffies.nice;
        diffJiffies.system = curJiffies.system - prevJiffies.system;
        diffJiffies.idle = curJiffies.idle - prevJiffies.idle;

        int totalJiffies = diffJiffies.user + diffJiffies.nice +
                           diffJiffies.system + diffJiffies.idle;

        diffJiffies.usage = 100.0f * (1.0 - (diffJiffies.idle / (double)totalJiffies));

        prevJiffies = curJiffies;
        fclose(pStat);
        sleep(1);

        reply->set_cpu_reply(request->cpu_request() + std::to_string(diffJiffies.usage) + '%');

        return Status::OK;
    }

    Status current_disk_usage_moniter_method(ServerContext *context,
                                             const DiskMoniterRequest *request,
                                             DiskMoniterReply *reply) override
    {

        long long avail, total, free;
        avail.QuadPart = 0L;
        total.QuadPart = 0L;
        free.QuadPart = 0L;

        int m_avail, m_total, m_free, m_used;

        ////////// Drive C
        // C:\의 하드디스크 용량 정보를 받아 옴
        GetDiskFreeSpaceEx(TEXT("c:\\"), &avail, &total, &free);

        // GByte 로 표현을 하기 위한 부분
        m_total = (int)(total.QuadPart >> 30);
        m_free = (int)(free.QuadPart >> 30);

        ////////// Drive D
        // D:\의 하드디스크 용량 정보를 받아 옴
        GetDiskFreeSpaceEx(TEXT("d:\\"), &avail, &total, &free);

        // GByte 로 표현을 하기 위한 부분
        m_total = (int)(total.QuadPart >> 30);
        m_free = (int)(free.QuadPart >> 30);
        m_used = m_total - m_free;

        std::string totalDisk_suffix(std::to_string(m_total)); // total disk volume을 받는 변수
        std::string usedDisk_suffix(std::to_string(m_used));   // used disk volume을 받는 변수
        std::string availDisk_suffix(std::to_string(m_free));  // available disk volume을 받는 변수

        reply->set_disk_info_reply(request->total_disk_volume_request() + totalDisk_suffix + "GB\n" + request->disk_usage_request() + usedDisk_suffix + "GB\n" + request->avail_disk_volume_request() + availDisk_suffix + "GB");
        return Status::OK;
    }
};