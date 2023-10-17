cmake_minimum_required(VERSION 3.10)

project(ASE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_ARCHITECTURES arm64)

set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl)
set(OPENSSL_LIBRARIES /opt/homebrew/opt/openssl/lib)

find_package(OpenSSL 1.1 REQUIRED)
find_package(cpprestsdk REQUIRED)

add_executable(server main/server.cpp)
target_link_libraries(server PRIVATE cpprestsdk::cpprest ${OPENSSL_LIBRARIES})

add_executable(client main/client.cpp)
target_link_libraries(client PRIVATE cpprestsdk::cpprest ${OPENSSL_LIBRARIES})


