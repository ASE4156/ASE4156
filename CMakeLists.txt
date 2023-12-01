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
include_directories(${CPPRESTSDK_INCLUDE_DIRS})
find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)

pkg_check_modules(PQXX REQUIRED libpqxx)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/api-endpoints
    ${PROJECT_SOURCE_DIR}/src/config
    ${PROJECT_SOURCE_DIR}/src/service
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/postgresql
    ${Boost_INCLUDE_DIRS}
)

# Define source files
set (SOURCES 
    src/main.cpp
    src/api-endpoints/Conversation.cpp
    src/api-endpoints/Token.cpp
    src/api-endpoints/Prompt.cpp
    src/api-endpoints/User.cpp
    src/service/ChatGPTService.cpp
    src/service/Authenticator.cpp
    src/config/AppConfig.cpp
    postgresql/db.cpp
)

set (TEST_SOURCES
    testing/AppConfigTest.cpp
    testing/ChatGPTServiceTest.cpp
    testing/MockChatGPTService.cpp
    testing/ConversationEndpointTest.cpp
    testing/PromptEndpointTest.cpp
    testing/AuthenticatorTest.cpp
    src/api-endpoints/Conversation.cpp
    src/api-endpoints/Prompt.cpp
    src/service/ChatGPTService.cpp
    src/service/Authenticator.cpp
    src/config/AppConfig.cpp
    postgresql/db.cpp
)

add_executable(server ${SOURCES})
target_link_libraries(server PRIVATE ${PQXX_LINK_LIBRARIES} cpprestsdk::cpprest ${OPENSSL_LIBRARIES})

#add_executable(client src/client/client.cpp)
#target_link_libraries(client PRIVATE cpprestsdk::cpprest ${OPENSSL_LIBRARIES})

add_executable(unit_test ${TEST_SOURCES} testing/unit_test.cpp  )
target_link_libraries(unit_test PRIVATE cpprestsdk::cpprest ${OPENSSL_LIBRARIES} ${PQXX_LINK_LIBRARIES})

option(CODE_COVERAGE "Enable coverage reporting" OFF)

if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -O0 -g)
    link_libraries(gcov)
endif()

