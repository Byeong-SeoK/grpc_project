#include "string"
#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#ifdef BAZEL_BUILD
#include "resource_moniter/protos/moniter.grpc.pb.h"
#else
#include "moniter.grpc.pb.h"
#endif

class SetlogDir
{
public:
    std::string setDir(); // 로그 기록이 저장될 파일 디렉토리를 지정
};