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
    voic save_level_Log(std::string log, int level);
}