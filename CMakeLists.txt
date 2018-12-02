cmake_minimum_required(VERSION 3.12)
project(litbase)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(Utils.cmake)

init_debug()

add_subdirectory(litcore)
add_subdirectory(litaudio)
add_subdirectory(litaudiofile)
add_subdirectory(litsignal)
add_subdirectory(test)