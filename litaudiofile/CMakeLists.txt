project(litaudiofile C CXX)

select_source_files()

include_directories(${litaudio_INCLUDE_DIRS})

set(USED_LIBRARIES
        litaudio)

init_library(${PROJECT_NAME} "${USED_LIBRARIES}")
