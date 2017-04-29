cmake_minimum_required(VERSION 3.5.1)
project(thread_pool CXX)

set(CMAKE_CXX_STANDARD 14)

set(
        CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                        -Werror \
                        -Wall \
                        -Wextra \
                        -O3"
)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
)

file(
        GLOB_RECURSE
        SOURCE_FILES
        ./src/*.cpp
)

file(
        GLOB_RECURSE
        HEADERS
        ./src/*.hh
)

add_executable(thread_pool ${SOURCE_FILES} ${HEADERS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(
        thread_pool Threads::Threads
        ${GTKMM_LIBRARIES}
)