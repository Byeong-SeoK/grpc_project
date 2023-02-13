#include "../../include/moniter_server.h"
#include "../../include/set_log_dir.h"

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

using moniter::DiskMoniterReply;
using moniter::DiskMoniterRequest;

using moniter::MoniterService;

Status MoniterServiceImpl::current_disk_usage_moniter_method(ServerContext *context,
                                                             const DiskMoniterRequest *request,
                                                             DiskMoniterReply *reply)
{
    SetlogDir dir;
    std::string logDir = dir.setDir();

    FLAGS_log_dir = logDir;
    mkdir(logDir.c_str(), 0755);

    LOG(INFO) << "Disk monitoring API start .";

    const unsigned int GB = (1024 * 1024) * 1024;
    struct statvfs buffer; // buffer라는 statvfs type 객체를 만든다. stavfs를 통해서 linux에 mount된 disk의 크기를 읽을 수 있다.
    try
    {
        statvfs("/", &buffer); // buffer에 root 하위 directory에 mount된 disk의 크기를 읽어오고 이 값은 statvfs type이다.
    }
    catch (...)
    {
        LOG(ERROR) << "Cannot find the directory";
        google::FlushLogFiles(google::GLOG_ERROR);
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Cannot find the directory");
    }

    const long total = (double)(buffer.f_blocks * buffer.f_bsize) / GB;     // f_frsize 단위의 fs 크기 * 파일 시스템 블록 크기 = 전체 disk 크기
    const long available = (double)(buffer.f_bavail * buffer.f_bsize) / GB; // 비특권 사용자를 위한 유휴 블록 수 * 파일 시스템 블록 크기 = 사용 가능한 disk 크기
    const long used = total - available;                                    // 전체 - 사용 가능 = 현재 사용중인 disk 크기

    std::string totalDisk_suffix(std::to_string(total));     // total disk volume을 받는 변수
    std::string usedDisk_suffix(std::to_string(used));       // used disk volume을 받는 변수
    std::string availDisk_suffix(std::to_string(available)); // available disk volume을 받는 변수

    reply->set_disk_info_reply(request->total_disk_volume_request() + totalDisk_suffix + "GB\n" + request->disk_usage_request() +
                               usedDisk_suffix + "GB\n" + request->avail_disk_volume_request() + availDisk_suffix + "GB");

    LOG(INFO) << "Disk monitoring API end .";

    google::FlushLogFiles(google::GLOG_INFO);

    return Status::OK;
}