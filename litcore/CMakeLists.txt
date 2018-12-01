project(litcore C CXX)

select_source_files()

message(STATUS ${SOURCE_FILES})
init_library(${PROJECT_NAME} ABS)
