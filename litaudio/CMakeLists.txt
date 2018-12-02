project(litaudio C CXX)

select_source_files()

find_package(FFmpeg REQUIRED)
include_directories(${FFMPEG_INCLUDE_DIR})

include_directories(${litcore_INCLUDE_DIRS})

set(USED_LIBRARIES
        ${FFMPEG_LIBRARIES}
        litcore)

init_library(${PROJECT_NAME} "${USED_LIBRARIES}")
