cmake_minimum_required(VERSION 3.7)
project(Reconstruction)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE sources src/**.cpp)
file(GLOB_RECURSE headers src/**.h)

set(MVE_BUILD_DIR ${CMAKE_BINARY_DIR}/libs/mve)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/libs/gnuplot-iostream)
include_directories(${CMAKE_SOURCE_DIR}/libs/eigen)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/gnuplot-iostream)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/eigen)

find_package(Boost REQUIRED COMPONENTS iostreams)

add_executable(${PROJECT_NAME} ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::iostreams )

add_custom_command(
        OUTPUT ${MVE_BUILD_DIR}/libmve.a
        COMMAND make -C ${CMAKE_SOURCE_DIR}/libs/mve -j8
        WORKING_DIRECTORY ${MVE_BUILD_DIR}
        COMMENT "Building MVE..."
)

add_custom_target(MVE ALL DEPENDS ${MVE_BUILD_DIR}/libmve.a)

set(MVE_BIN
        ${CMAKE_SOURCE_DIR}/libs/mve/apps/dmrecon/dmrecon
        ${CMAKE_SOURCE_DIR}/libs/mve/apps/scene2pset/scene2pset
        ${CMAKE_SOURCE_DIR}/libs/mve/apps/meshclean/meshclean
        ${CMAKE_SOURCE_DIR}/libs/mve/apps/makescene/makescene
        ${CMAKE_SOURCE_DIR}/libs/mve/apps/fssrecon/fssrecon
        ${CMAKE_SOURCE_DIR}/libs/mve/apps/sfmrecon/sfmrecon
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/../bin_mve/)

add_custom_command(
        TARGET MVE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${MVE_BIN} ${CMAKE_BINARY_DIR}/../bin_mve/
        COMMENT "Copying compiled .a files to ${CMAKE_BINARY_DIR}/bin_mve/"
)

add_dependencies(${PROJECT_NAME} MVE)