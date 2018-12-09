project(litsignal C CXX)

select_source_files()

find_package(FFTW REQUIRED)
add_definitions(-DHAVE_FFTW)
find_package(Armadillo REQUIRED)

include_directories(${litaudio_INCLUDE_DIRS})
include_directories(${ARMADILLO_INCLUDE_DIR})
include_directories(${FFTW_INCLUDE_DIRS})
include_directories(${SIGPACK_INCLUDE_DIR})

set(USED_LIBRARIES
        litaudio
        ${ARMADILLO_LIBRARIES}
        ${FFTW_LIBRARIES}
        )

init_library(${PROJECT_NAME} "${USED_LIBRARIES}")