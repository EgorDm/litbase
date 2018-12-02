cmake_minimum_required(VERSION 3.12)
project(litbase)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(Utils.cmake)

init_debug()

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
set(SIGPACK_INCLUDE_DIR "${LIB_DIR}/sigpack")

add_subdirectory(litcore)
add_subdirectory(litaudio)
add_subdirectory(litaudiofile)
add_subdirectory(litsignal)
add_subdirectory(test)