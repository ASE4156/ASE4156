cmake_minimum_required(VERSION 3.10)

project(ASE)

set(CMAKE_CXX_STANDARD 17)

### set os architecture variable ###
if(EXISTS "${CMAKE_SOURCE_DIR}/LocalConfig.cmake")
    include(${CMAKE_SOURCE_DIR}/LocalConfig.cmake)
else()
    # set a default value if LocalConfig.cmake doesn't exist
    set(MY_MACOS_ARCH "x86_64")
endif()
set(CMAKE_OSX_ARCHITECTURES ${MY_MACOS_ARCH})

set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl)
set(OPENSSL_LIBRARIES /opt/homebrew/opt/openssl/lib)

find_package(OpenSSL 1.1 REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(PQXX REQUIRED libpqxx)


set (SOURCES main/server.cpp main/http_client_util.cpp main/env_util.cpp)

add_executable(server ${SOURCES})
target_link_libraries(server PRIVATE ${PQXX_LINK_LIBRARIES} cpprestsdk::cpprest ${OPENSSL_LIBRARIES})

add_executable(client main/client.cpp)
target_link_libraries(client PRIVATE cpprestsdk::cpprest ${OPENSSL_LIBRARIES})

add_executable(unit_test testing/unit_test.cpp)
target_link_libraries(unit_test PRIVATE cpprestsdk::cpprest ${OPENSSL_LIBRARIES})



