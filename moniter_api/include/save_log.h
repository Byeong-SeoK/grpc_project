#include "string"
#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

class SaveLog
{
public:
    void save_level_Log(int level, const char *log); // level 파라미터에는 google::* 룰 받고 log에는 기록으로 남길 sentence를 받는다.
};