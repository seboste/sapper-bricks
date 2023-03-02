add_library(handler-grpc)

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

#for documentation of protobuf_generate see: https://medium.com/3yourmind/generate-grpc-source-files-using-cmake-ec3acd246b45

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(PROTO_IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}")

file(MAKE_DIRECTORY ${PROTO_BINARY_DIR})

protobuf_generate(
  LANGUAGE cpp  
  TARGET handler-grpc
  PROTOS api.proto
  IMPORT_DIRS ${PROTO_IMPORT_DIRS}
  PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)

protobuf_generate(  
  LANGUAGE grpc 
  TARGET handler-grpc
  PROTOS api.proto
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${GRPC_CPP_PLUGIN_PROGRAM}"
  IMPORT_DIRS ${PROTO_IMPORT_DIRS}
  PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)

target_include_directories(handler-grpc PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

target_sources(handler-grpc
    PUBLIC
        handler-grpc.h
        handler-grpc.cpp
    )
    
target_link_libraries(handler-grpc
    PUBLIC
        ports    
        microservice-essentials::microservice-essentials                
        protobuf::protobuf
        gRPC::gRPC
    )
