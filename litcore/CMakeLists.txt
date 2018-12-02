project(litcore C CXX)

select_source_files()

set(USED_LIBRARIES "")

init_library(${PROJECT_NAME} "${USED_LIBRARIES}")
