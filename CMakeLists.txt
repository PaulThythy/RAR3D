cmake_minimum_required(VERSION 3.7)
project(Reconstruction)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE sources src/**.cpp)
file(GLOB_RECURSE headers src/**.h)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(Boost REQUIRED COMPONENTS iostreams)

add_executable(${PROJECT_NAME} ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::iostreams)