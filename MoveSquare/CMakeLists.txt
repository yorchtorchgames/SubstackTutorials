cmake_minimum_required(VERSION 3.22)
project(MovingSquare LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL3_DIR "C:/SDL3" CACHE PATH "Path to extracted SDL3-dev-x.zip")

list(APPEND CMAKE_PREFIX_PATH "${SDL3_DIR}")

find_package(SDL3 REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
