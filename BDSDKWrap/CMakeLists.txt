cmake_minimum_required(VERSION 3.13)
project(BDSpeedSDKWrap)

set(CMAKE_CXX_STANDARD 17)

set(WrapFiles SDKWrap/Callback.hpp SDKWrap/DefaultCallback.cpp SDKWrap/ProductId.cpp
        SDKWrap/Sdk.cpp SDKWrap/SdkConfig.cpp SDKWrap/Message/CancelMessage.hpp SDKWrap/Message/ErrorMessage.cpp
        SDKWrap/Message/Message.cpp  SDKWrap/Message/PushMessage.cpp  SDKWrap/Message/ReceiveMessage.hpp
        SDKWrap/Message/RecognitionEndMessage.hpp  SDKWrap/Message/RecognitionStartMessage.hpp
        SDKWrap/Message/SentenceFinishData.cpp  SDKWrap/Message/SentenceFlushData.cpp
        SDKWrap/Message/SentenceStartMessage.hpp  SDKWrap/Message/UnknowMessage.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Library/BDSpeedSDK/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Library/BDSpeedSDK/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Library/BDSpeedSDK/extern/lib)

add_compile_options(-D_GLIBCXX_USE_CXX11_ABI=0)


# Find Protobuf installation
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
# Find gRPC installation
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(ProjectProto "../Protos/Project.proto" ABSOLUTE)
get_filename_component(ProjectProtoPath "${ProjectProto}" PATH)

# Generated sources
set(ProjectProtoSrcs "${CMAKE_CURRENT_BINARY_DIR}/project.pb.cc")
set(ProjectProtoHdrs "${CMAKE_CURRENT_BINARY_DIR}/project.pb.h")
set(ProjectGrpcSrcs "${CMAKE_CURRENT_BINARY_DIR}/project.grpc.pb.cc")
set(ProjectGrpcHdrs "${CMAKE_CURRENT_BINARY_DIR}/project.grpc.pb.h")
add_custom_command(
        OUTPUT "${ProjectProtoSrcs}" "${ProjectProtoHdrs}" "${ProjectGrpcSrcs}" "${ProjectGrpcHdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${ProjectProtoPath}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${ProjectProto}"
        DEPENDS "${ProjectProto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

#库链接顺序需要注意
set(ProjectLinkLibrarys BDSpeechSDK curl ssl pthread dl crypto uuid z iconv gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc_cronet)

add_executable(BDSpeedSDKWraper main.cpp ${WrapFiles} ${ProjectProtoSrcs} ${ProjectGrpcSrcs})
target_link_libraries(BDSpeedSDKWraper ${ProjectLinkLibrarys})

