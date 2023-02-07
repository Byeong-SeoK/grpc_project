# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ memory moniter project.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building memory moniter.

cmake_minimum_required(VERSION 3.5.1)

project(Moniter C CXX)

#find_package (glog 0.3.5)

include(../../cmake/common.cmake)

include(../../cmake/FindGlog.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(glog0.3.5 REQUIRED libglog)

find_package(GFLAGS REQUIRED)


# Monitering API Proto file
get_filename_component(mm_proto "../../protos/moniter.proto" ABSOLUTE)
get_filename_component(mm_proto_path "${mm_proto}" PATH)


# Generated monitering api sources
set(mm_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/moniter.pb.cc")
set(mm_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/moniter.pb.h")
set(mm_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/moniter.grpc.pb.cc")
set(mm_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/moniter.grpc.pb.h")
add_custom_command(
      OUTPUT "${mm_proto_srcs}" "${mm_proto_hdrs}" "${mm_grpc_srcs}" "${mm_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${mm_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${mm_proto}"
      DEPENDS "${mm_proto}")


# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


# mm_grpc_proto
add_library(mm_grpc_proto
  ${mm_grpc_srcs}
  ${mm_grpc_hdrs}
  ${mm_proto_srcs}
  ${mm_proto_hdrs})
target_link_libraries(mm_grpc_proto
  ${GLOG_LIBRARIES}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})


add_library(system_log_API
  "../include/set_log_dir.h"
  "./log/set_log_dir.cc"
)
target_link_libraries(system_log_API
  ${GLOG_LIBRARIES}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)


# include monitering API header of server part
add_library(system_monitering_server_API
  "../include/moniter_server.h"
  "./memory/memory_server.cc"
  "./cpu/cpu_server.cc"
  "./disk/disk_server.cc"
  "./process/all_process_server.cc"
  "./process/selected_process_server.cc"
)
target_link_libraries(system_monitering_server_API
  ${GLOG_LIBRARIES}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})




#include cpu monitoring API header
add_library(system_monitering_client_API #외부 사용자 정의 class의 method를 구현할 때는 관련된 header파일과 cc파일이 상대 경로로 들어가야 한다. 
    "../include/moniter_client.h"
    "./memory/memory_client.cc"
    "./cpu/cpu_client.cc"
    "./disk/disk_client.cc"
    "./process/all_process_client.cc"
    "./process/selected_process_client.cc"
)
#process_client.h에 class가 2개라 해서 add_library를 2개를 사용하면 안된다. Compile, build, linking할 때는
#header 파일 1개가 하나의 라이브러리에 해당하므로 add_library를 2개를 사용하면 첫번째 add_library에서는
#아래 쪽 class를 build하지 못하여 오류가 발생하고 두번째 add_library에서는 위 쪽 class를 build하지 못하여 오류가 발생한다. 
target_link_libraries(system_monitering_client_API #add_libaray 이후 target_link를 해야 build와 linking이 한번에 가능하도록 만들 수 있다.
  ${GLOG_LIBRARIES}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)


add_library(client_UI
  "../include/client_UI.h"
  "./UI/client_UI.cc"
)
target_link_libraries(system_monitering_client_API #add_libaray 이후 target_link를 해야 build와 linking이 한번에 가능하도록 만들 수 있다.
  ${GLOG_LIBRARIES}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)


#Targets [async_](client|server)
foreach(_target
    client server)
add_executable(${_target} "${_target}.cc")
target_link_libraries(${_target}
    mm_grpc_proto
                        #memory_moniter_API 
                        #여기서는 system_moniter_API가 아닌 현 cmakelist에 정의된 memory_moniter_API가 들어가야한다. 
                        #왜냐하면 system_moniter_API는 부모 cmake이므로 여기에서 해당 파일을 알 수 가 없다.
                        #다만 하위 directory의 cmake에서 만든 alias는 참조가 가능하다.
    
    system_monitering_server_API
    system_monitering_client_API
    system_log_API
    client_UI
    ${GLOG_LIBRARIES}
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()

#client에서 참조하는 함수가 memory_client에 정의되어 있으므로 이것을 client 보다 먼저 linking되도록 해야한다.
#cmake는 위에서 부터 아래로 왼쪽에서 부터 오른쪽으로 의존 관계와 무관하게 build하므로 이를 유념하여 작성해야한다.