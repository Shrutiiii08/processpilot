cmake_minimum_required(VERSION 3.16)

project(ProcessPilot
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(processpilot
    ${SOURCES}
)

target_link_libraries(processpilot
    PRIVATE pthread
)
