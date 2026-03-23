cmake_minimum_required(VERSION 3.21.0)
project(Rest VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#find_package(redis++ CONFIG REQUIRED)

# 1️⃣ Executable
add_executable(rest
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  ${CMAKE_SOURCE_DIR}/src/ui.cpp
  ${CMAKE_SOURCE_DIR}/src/keys.cpp
  ${CMAKE_SOURCE_DIR}/src/appdata.cpp
)

target_compile_options(rest PRIVATE -Wall -Wextra -Werror -Wno-pedantic)
target_include_directories(rest PRIVATE "${PROJECT_SOURCE_DIR}/include")

#target_link_libraries(logger PRIVATE redis++::redis++_static)
