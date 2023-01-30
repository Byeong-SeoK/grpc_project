cmake_minimum_required(VERSION 3.5.1)

project(Moniter C CXX)

add_library(memory_moniter_API #외부 사용자 정의 class의 method를 구현할 때는 관련된 header파일과 cc파일이 상대 경로로 들어가야 한다. 
    "../include/memory_client.h"
    "./memory_client.cc"
)
target_link_libraries(memory_moniter_API #add_libaray 이후 target_link를 해야 build와 linking이 한번에 가능하도록 만들 수 있다.
  ${glog_LIBRARIES}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

#Targets [async_](client|server)
foreach(_target
    client server )
add_executable(${_target} "${_target}.cc")
target_link_libraries(${_target}
    mm_grpc_proto
    memory_moniter_API #여기서는 system_moniter_API가 아닌 현 cmakelist에 정의된 memory_moniter_API가 들어가야한다. 
                       #왜냐하면 system_moniter_API는 부모 cmake이므로 여기에서 해당 파일을 알 수 가 없다.
    ${glog_LIBRARIES}
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()

#client에서 참조하는 함수가 memory_client에 정의되어 있으므로 이것을 client 보다 먼저 linking되도록 해야한다.
#cmake는 위에서 부터 아래로 왼쪽에서 부터 오른쪽으로 의존 관계와 무관하게 build하므로 이를 유념하여 작성해야한다.