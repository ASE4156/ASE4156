cmake_minimum_required(VERSION 3.10)

project(HelloWorldGRPC)

set(CMAKE_CXX_STANDARD 11)

find_package(gRPC REQUIRED)

# Create the server executable
add_executable(server src/server.cpp hello.pb.cc hello.grpc.pb.cc)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(server gRPC::grpc++ gRPC::grpc++_reflection)


add_executable(client src/client.cpp hello.pb.cc hello.grpc.pb.cc)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(client gRPC::grpc++ gRPC::grpc++_reflection)