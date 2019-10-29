cmake_minimum_required(VERSION 3.14)

find_package(PkgConfig)

set(PROJECT_NAME Solver)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unknown-pragmas -fconstexpr-depth=-1 -fconstexpr-steps=-1")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -DDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG -march=x86-64")

set(Boost_NO_BOOST_CMAKE ON)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     ON)

find_package(Boost REQUIRED COMPONENTS system program_options)

add_executable(${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/src/field.cpp
  ${PROJECT_SOURCE_DIR}/src/agent.cpp
  )

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES})

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS})
