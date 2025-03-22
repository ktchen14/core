cmake_minimum_required(VERSION 3.13)

file(READ VERSION PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
project("Vector"
  VERSION "${PROJECT_VERSION}"
  DESCRIPTION "An unobtrusive coroutine implementation in C"
  HOMEPAGE_URL https://github.com/ktchen14/core
  LANGUAGES C ASM_NASM)

add_library(core
  header/core.h
  source/core-x86_64.asm
  source/core-x86_64.c
  source/core.c)
target_compile_features(core PUBLIC c_std_11)
target_compile_options(core PRIVATE -Werror -Wall)
target_include_directories(core PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/header>"
  "$<INSTALL_INTERFACE:include>")
