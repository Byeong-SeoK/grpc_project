#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
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

using moniter::ProcessMoniterReply;
using moniter::ProcessMoniterRequest;

using moniter::SelectedProcessMoniterReply;
using moniter::SelectedProcessMoniterRequest;

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

        pStat = fopen("/proc/stat", "r"); // proc/stat을 read 모드로 열어서 cpu 상태를 읽어와 pStat에 저장한다.
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

        return Status::OK;
    }

    Status current_disk_usage_moniter_method(ServerContext *context,
                                             const DiskMoniterRequest *request,
                                             DiskMoniterReply *reply) override
    {
        const unsigned int GB = (1024 * 1024) * 1024;
        struct statvfs buffer; // buffer라는 statvfs type 객체를 만든다. stavfs를 통해서 linux에 mount된 disk의 크기를 읽을 수 있다.
        statvfs("/", &buffer); // buffer에 root 하위 directory에 mount된 disk의 크기를 읽어오고 이 값은 statvfs type이다.

        const long total = (double)(buffer.f_blocks * buffer.f_bsize) / GB;     // f_frsize 단위의 fs 크기 * 파일 시스템 블록 크기 = 전체 disk 크기
        const long available = (double)(buffer.f_bavail * buffer.f_bsize) / GB; // 비특권 사용자를 위한 유휴 블록 수 * 파일 시스템 블록 크기 = 사용 가능한 disk 크기
        const long used = total - available;                                    // 전체 - 사용 가능 = 현재 사용중인 disk 크기

        std::string totalDisk_suffix(std::to_string(total));     // total disk volume을 받는 변수
        std::string usedDisk_suffix(std::to_string(used));       // used disk volume을 받는 변수
        std::string availDisk_suffix(std::to_string(available)); // available disk volume을 받는 변수

        reply->set_disk_info_reply(request->total_disk_volume_request() + totalDisk_suffix + "GB\n" + request->disk_usage_request() + usedDisk_suffix + "GB\n" + request->avail_disk_volume_request() + availDisk_suffix + "GB");
        return Status::OK;
    }

    Status current_process_moniter_method(ServerContext *context,
                                          const ProcessMoniterRequest *request,
                                          ProcessMoniterReply *reply) override
    {
        char buffer[128];                  // pipe를 통해서 shell창의 출력 결과를 임시적으로 저장하는 buffer이다.
        std::string all_process_info = ""; // shell창의 출력 결과를 받는 변수
        FILE *pipe = popen("ps -ef", "r"); // shell창에 ps라는 명령어를 내리고 그 결과를 read mode로 읽어온다.
        if (!pipe)
            throw std::runtime_error("popen() failed!");
        try
        {
            while (fgets(buffer, sizeof(buffer), pipe) != NULL) // buffer에 저장된 것 끝까지 하나씩 읽어서 all_process_info에 저장한다.
            {
                all_process_info += buffer;
            }
        }
        catch (...)
        {
            pclose(pipe);
            throw;
        }

        std::string process_pid(std::to_string(getpid()));         // 현재 process의 PID를 받는다.
        std::string parent_process_pid(std::to_string(getppid())); // 현재 process의 parent process의 PID를 받는다.

        reply->set_process_info_reply(request->process_info_request() + process_pid + "\n" + request->parent_process_info_request() + parent_process_pid + "\n" + request->all_process_info_request() + "\n" + all_process_info);

        return Status::OK;
    }

    Status selected_process_moniter_method(ServerContext *context,
                                           const SelectedProcessMoniterRequest *request,
                                           SelectedProcessMoniterReply *reply) override
    {
        char pid_buffer[128];
        std::string pid_process_info = "";
        std::string temp = "ps -ef | grep " + request->pid_number();
        const char *command = temp.c_str();
        FILE *pipe2 = popen(command, "r");
        if (!pipe2)
            throw std::runtime_error("popen() failed!");
        try
        {
            while (fgets(pid_buffer, sizeof(pid_buffer), pipe2) != NULL)
            {
                pid_process_info = pid_process_info + pid_buffer;
            }
        }
        catch (...)
        {
            pclose(pipe2);
            throw;
        }

        reply->set_selected_process_info_reply(request->pid_process_info_request() + pid_process_info);

        return Status::OK;
    }
};