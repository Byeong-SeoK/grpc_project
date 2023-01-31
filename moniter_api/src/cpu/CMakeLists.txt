cmake_minimum_required(VERSION 3.5.1)

project(Moniter C CXX)

add_library(cpu_moniter_API #외부 사용자 정의 class의 method를 구현할 때는 관련된 header파일과 cc파일이 상대 경로로 들어가야 한다. 
    "../../include/cpu_client.h"
    "./cpu_client.cc"
)
target_link_libraries(cpu_moniter_API #add_libaray 이후 target_link를 해야 build와 linking이 한번에 가능하도록 만들 수 있다.
  ${glog_LIBRARIES}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)