cmake_minimum_required(VERSION 3.8)

project(base)

message("-- system arch:  ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "current platform: ${CMAKE_SYSTEM_NAME} ")

add_definitions(-DOS_ARCH=${CMAKE_SYSTEM_NAME})

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
    message(STATUS "Compile Debug")
ELSE()
    add_definitions(-DRELEASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread")
    message(STATUS "Compile Release")
ENDIF()

IF (WIN32)
    add_definitions(-DOS_WIN=1)
ELSEIF (APPLE)
    add_definitions(-DOS_LINUX=1)
ELSEIF (UNIX)
    add_definitions(-DOS_LINUX=1)
ENDIF ()

IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_WIN32_WINNT=0x0601 /W4 /wd4068 /wd4702 /wd4996" )

    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
ENDIF()
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_OUTPUT_DIR ${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
message("-- output path:  ${PROJECT_OUTPUT_DIR} ")

file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)

set(
    DepSrc
    ${PROJECT_SOURCE_DIR}/src/base/third_party/cityhash/city.cc
    ${PROJECT_SOURCE_DIR}/src/base/third_party/cityhash/city.h
    ${PROJECT_SOURCE_DIR}/src/base/third_party/crc32/crc32.hpp
    ${PROJECT_SOURCE_DIR}/src/base/third_party/modp_b64/modp_b64_data.h
    ${PROJECT_SOURCE_DIR}/src/base/third_party/modp_b64/modp_b64.cc
    ${PROJECT_SOURCE_DIR}/src/base/third_party/modp_b64/modp_b64.h
)

set(
    ProgrameSrc
    ${DepSrc}
    ${PROJECT_SOURCE_DIR}/src/base/at_exit.cc
    ${PROJECT_SOURCE_DIR}/src/base/at_exit.h
    ${PROJECT_SOURCE_DIR}/src/base/base_export.h
    ${PROJECT_SOURCE_DIR}/src/base/encode/base64.cc
    ${PROJECT_SOURCE_DIR}/src/base/encode/base64.h
    ${PROJECT_SOURCE_DIR}/src/base/file/file_utils.cc
    ${PROJECT_SOURCE_DIR}/src/base/file/file_utils.h
    ${PROJECT_SOURCE_DIR}/src/base/hash/crc32.cc
    ${PROJECT_SOURCE_DIR}/src/base/hash/crc32.h
    ${PROJECT_SOURCE_DIR}/src/base/hash/hash.cc
    ${PROJECT_SOURCE_DIR}/src/base/hash/hash.h
    ${PROJECT_SOURCE_DIR}/src/base/hash/md5.cc
    ${PROJECT_SOURCE_DIR}/src/base/hash/md5.h
    ${PROJECT_SOURCE_DIR}/src/base/log/logging.cc
    ${PROJECT_SOURCE_DIR}/src/base/log/logging.h
    ${PROJECT_SOURCE_DIR}/src/base/macros.h
    ${PROJECT_SOURCE_DIR}/src/base/singleton.h
    ${PROJECT_SOURCE_DIR}/src/base/string/string_convert.cc
    ${PROJECT_SOURCE_DIR}/src/base/string/string_convert.h
    ${PROJECT_SOURCE_DIR}/src/base/thread/closure.cc
    ${PROJECT_SOURCE_DIR}/src/base/thread/closure.h
    ${PROJECT_SOURCE_DIR}/src/base/thread/message_loop_manager.cc
    ${PROJECT_SOURCE_DIR}/src/base/thread/message_loop_manager.h
    ${PROJECT_SOURCE_DIR}/src/base/thread/message_loop.cc
    ${PROJECT_SOURCE_DIR}/src/base/thread/message_loop.h
    ${PROJECT_SOURCE_DIR}/src/base/thread/task_queue.cc
    ${PROJECT_SOURCE_DIR}/src/base/thread/task_queue.h
    ${PROJECT_SOURCE_DIR}/src/base/thread/thread_pool.cc
    ${PROJECT_SOURCE_DIR}/src/base/thread/thread_pool.h
    ${PROJECT_SOURCE_DIR}/src/base/thread/thread.cc
    ${PROJECT_SOURCE_DIR}/src/base/thread/thread.h
    ${PROJECT_SOURCE_DIR}/src/base/time/time.cc
    ${PROJECT_SOURCE_DIR}/src/base/time/time.h
    ${PROJECT_SOURCE_DIR}/src/base/timer/delay_timer.cc
    ${PROJECT_SOURCE_DIR}/src/base/timer/delay_timer.h
    ${PROJECT_SOURCE_DIR}/src/base/timer/elapsed_timer.cc
    ${PROJECT_SOURCE_DIR}/src/base/timer/elapsed_timer.h
    ${PROJECT_SOURCE_DIR}/src/base/utils.cc
    ${PROJECT_SOURCE_DIR}/src/base/utils.h
    ${PROJECT_SOURCE_DIR}/src/base/json/json.cc
    ${PROJECT_SOURCE_DIR}/src/base/json/json.h
    ${PROJECT_SOURCE_DIR}/src/base/array/tree_array.h
    ${PROJECT_SOURCE_DIR}/src/base/rand_util.cc
    ${PROJECT_SOURCE_DIR}/src/base/rand_util.h
    ${PROJECT_SOURCE_DIR}/src/base/guid.cc
    ${PROJECT_SOURCE_DIR}/src/base/guid.h
    ${PROJECT_SOURCE_DIR}/src/base/string/string_utils.cc
    ${PROJECT_SOURCE_DIR}/src/base/string/string_utils.h
    ${PROJECT_SOURCE_DIR}/src/base/string/stringprintf.cc
    ${PROJECT_SOURCE_DIR}/src/base/string/stringprintf.h
    ${PROJECT_SOURCE_DIR}/src/base/stl_util.h
)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(
        ${PROJECT_NAME}
        STATIC
        ${ProgrameSrc})



# compile demo
set(
    demoSrc
    ${PROJECT_SOURCE_DIR}/src/demo/thread_test.cc
)

add_executable(demo ${demoSrc})
target_link_libraries(demo base)


#Unit test
add_subdirectory(src/base/third_party/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(
    unittestSrc
    src/base/testing/test_main.cc
    src/test/hash/crc32_unittest.cc
    src/test/hash/md5_unittest.cc
    src/test/json/json_unittest.cc
    src/test/guid/guid_unittest.cc
    src/test/guid/rand_unittest.cc
)

add_executable(base_unittest ${unittestSrc})
target_link_libraries(base_unittest gtest gtest_main)
target_link_libraries(base_unittest ${PROJECT_NAME})

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(NAME test COMMAND base_unittest)