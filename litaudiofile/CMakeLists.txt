project(litaudiofile C CXX)

select_source_files()

find_package(Taglib REQUIRED)

include_directories(${litaudio_INCLUDE_DIRS})
include_directories(${TAGLIB_INCLUDES})

set(USED_LIBRARIES
        litaudio
        ${TAGLIB_LIBRARIES})

init_library(${PROJECT_NAME} "${USED_LIBRARIES}")
