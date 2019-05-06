cmake_minimum_required(VERSION 3.13)
project(BDSpeedSDKWrap)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "Only Support gcc")
endif()
if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4")
    message(FATAL_ERROR "Gcc version must be greater than or equal to 4")
endif()

set(SDKWrapFiles SDKWrap/Callback.hpp SDKWrap/DefaultCallback.cpp SDKWrap/ProductId.cpp
        SDKWrap/Sdk.cpp SDKWrap/SdkConfig.cpp SDKWrap/Message/CancelMessage.hpp SDKWrap/Message/ErrorMessage.cpp
        SDKWrap/Message/Message.cpp  SDKWrap/Message/PushMessage.cpp  SDKWrap/Message/ReceiveMessage.hpp
        SDKWrap/Message/RecognitionEndMessage.hpp  SDKWrap/Message/RecognitionStartMessage.hpp
        SDKWrap/Message/SentenceFinishData.cpp  SDKWrap/Message/SentenceFlushData.cpp
        SDKWrap/Message/SentenceStartMessage.hpp  SDKWrap/Message/UnknowMessage.cpp)

add_compile_options(-D_GLIBCXX_USE_CXX11_ABI=0)
add_compile_options(-Wl,--exclude-libs,ALL)

add_library(BDSpeedSDKWrap SHARED EXCLUDE_FROM_ALL ${SDKWrapFiles})

set_target_properties(BDSpeedSDKWrap PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN true)
target_include_directories(BDSpeedSDKWrap PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Library/BDSpeedSDK/include)
target_link_directories(BDSpeedSDKWrap PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Library/BDSpeedSDK/lib
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Library/BDSpeedSDK/extern/lib)
target_link_libraries(BDSpeedSDKWrap BDSpeechSDK curl ssl pthread dl crypto uuid z iconv)


add_subdirectory(Library/gRPC ${CMAKE_CURRENT_BINARY_DIR}/grpc EXCLUDE_FROM_ALL)
message(STATUS "Using gRPC via add_subdirectory.")
set(_PROTOBUF_LIBPROTOBUF libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)

# Proto file
get_filename_component(ProjectProto "../Protos/Project.proto" ABSOLUTE)
get_filename_component(ProjectProtoPath "${ProjectProto}" PATH)

# Generated sources
set(ProjectProtoSrcs "${CMAKE_CURRENT_BINARY_DIR}/Project.pb.cc")
set(ProjectProtoHdrs "${CMAKE_CURRENT_BINARY_DIR}/Project.pb.h")
set(ProjectGrpcSrcs "${CMAKE_CURRENT_BINARY_DIR}/Project.grpc.pb.cc")
set(ProjectGrpcHdrs "${CMAKE_CURRENT_BINARY_DIR}/Project.grpc.pb.h")
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

add_executable(gRPCClient main.cpp ${ProjectProtoSrcs} ${ProjectGrpcSrcs})
target_link_libraries(gRPCClient ${_GRPC_GRPCPP_UNSECURE} ${_PROTOBUF_LIBPROTOBUF})
add_dependencies(gRPCClient BDSpeedSDKWrap)
