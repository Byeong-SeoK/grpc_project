cmake_minimum_required(VERSION 3.5.1)

project(Moniter C CXX)

add_library(process_moniter_API #외부 사용자 정의 class의 method를 구현할 때는 관련된 header파일과 cc파일이 상대 경로로 들어가야 한다. 
    "../../include/process_client.h"
    "./all_process_client.cc"
    "./selected_process_client.cc"
)
#process_client.h에 class가 2개라 해서 add_library를 2개를 사용하면 안된다. Compile, build, linking할 때는
#header 파일 1개가 하나의 라이브러리에 해당하므로 add_library를 2개를 사용하면 첫번째 add_library에서는
#아래 쪽 class를 build하지 못하여 오류가 발생하고 두번째 add_library에서는 위 쪽 class를 build하지 못하여 오류가 발생한다. 

target_link_libraries(
    process_moniter_API #add_libaray 이후 target_link를 해야 build와 linking이 한번에 가능하도록 만들 수 있다.
  ${glog_LIBRARIES}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)