cmake_minimum_required(VERSION 3.13)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tool")

file(READ VERSION PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
project("Core"
  VERSION "${PROJECT_VERSION}"
  DESCRIPTION "An unobtrusive coroutine implementation in C"
  HOMEPAGE_URL https://github.com/ktchen14/core
  LANGUAGES C)

# add_subdirectory(header)
# add_subdirectory(source)

add_library(core
  header/core.h
  source/core-x86_64.asm
  source/core-x86_64.h
  source/core.c)
target_compile_features(core PUBLIC c_std_11)
target_compile_options(core PRIVATE -Werror -Wall -Wextra)
target_include_directories(core PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/header>"
  "$<INSTALL_INTERFACE:include>")

install(TARGETS core
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
