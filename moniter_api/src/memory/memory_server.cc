#include "../../include/moniter_server.h"
#include "../../include/set_log_dir.h"

#include <iostream>
#include <fstream>
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

using moniter::MemoryReply;
using moniter::MemoryRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::current_memory_moniter_method(ServerContext *context,
                                                         const MemoryRequest *request,
                                                         MemoryReply *reply)
{
    fLB::FLAGS_logtostderr = 0; // 0으로 값을 주면 shell창과 파일 동시에 로그 기록이 남지만, 1으로 값을 주면 로그 기록이 파일에는 남지 않는다.

    SetlogDir dir;
    std::string logDir = dir.setDir();

    FLAGS_log_dir = logDir;
    mkdir(logDir.c_str(), 0755);

    LOG(INFO) << "Memory monitoring API start .";

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

    // availPhyMem = 128;
    // availVirMem = 128;

    std::string v_suffix(std::to_string(virtualMemUsed)); // 현재 사용중인 가상 메모리 용량을 받는 변수
    std::string p_suffix(std::to_string(physMemUsed));    // 현재 사용중인 물리적 메모리 용량을 받는 변수
    std::string avm_suffix(std::to_string(availVirMem));  // 현재 가용할 수 있는 가상 메모리 용량을 받는 변수
    std::string apm_suffix(std::to_string(availPhyMem));  // 현재 가용할 수 있는 물리적 메모리 용량을 받는 변수

    // availPhyMem = 128;
    // availVirMem = 128;
    if (availPhyMem < 256 && availVirMem > 256)
    {
        reply->set_memory_info_reply("\033[0;37m" + request->virtual_memory_request() + v_suffix + "MB\n" +
                                     request->physical_memory_request() + p_suffix + "MB\n" +
                                     request->avail_virtual_memory_request() + avm_suffix + "MB\n" +
                                     request->avail_physical_memory_request() + "\033[0;31m" + apm_suffix + "MB");

        std::string path = "../../../../alarm/memory_issue.txt";
        std::ofstream ofs;
        ofs.open(path, std::ios::app);
        ofs << request->avail_physical_memory_request() + apm_suffix + "MB" << std::endl;
    }
    else if (availPhyMem > 256 && availVirMem < 256)
    {
        reply->set_memory_info_reply("\033[0;37m" + request->virtual_memory_request() + v_suffix + "MB\n" +
                                     request->physical_memory_request() + p_suffix + "MB\n" +
                                     request->avail_virtual_memory_request() + "\033[0;31m" + avm_suffix + "MB\n" +
                                     request->avail_physical_memory_request() + apm_suffix + "MB");

        std::string path = "../../../../alarm/memory_issue.txt";
        std::ofstream ofs;
        ofs.open(path, std::ios::app);
        ofs << request->avail_virtual_memory_request() + avm_suffix + "MB" << std::endl;
    }
    else if (availPhyMem < 256 && availVirMem < 256)
    {
        reply->set_memory_info_reply("\033[0;37m" + request->virtual_memory_request() + v_suffix + "MB\n" +
                                     request->physical_memory_request() + p_suffix + "MB\n" +
                                     request->avail_virtual_memory_request() + "\033[0;31m" + avm_suffix + "MB\n" +
                                     "\033[0;37m" + request->avail_physical_memory_request() + "\033[0;31m" + apm_suffix + "MB");

        std::string path = "../../../../alarm/memory_issue.txt";
        std::ofstream ofs;
        ofs.open(path, std::ios::app);
        ofs << request->avail_virtual_memory_request() + avm_suffix + "MB" << std::endl;
        ofs << request->avail_physical_memory_request() + apm_suffix + "MB" << std::endl;
    }
    else
    {
        reply->set_memory_info_reply("\033[0;37m" + request->virtual_memory_request() + v_suffix + "MB\n" +
                                     request->physical_memory_request() + p_suffix + "MB\n" +
                                     request->avail_virtual_memory_request() + "\033[0;37m" + avm_suffix + "MB\n" +
                                     request->avail_physical_memory_request() + "\033[0;37m" + apm_suffix + "MB");
        // 이 set_*()의 인자는 request->* 하나만 받는다. 그래서 여러 개의 request->*를 넘기면 build 오류가 발생한다.
    }

    LOG(INFO) << "Memory monitoring API end .";

    google::FlushLogFiles(google::GLOG_INFO);

    return Status::OK;
}