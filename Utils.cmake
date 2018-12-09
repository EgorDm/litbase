function(define_lib_includes)
    get_property(tmp_include_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR} ${tmp_include_dirs}
            CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
endfunction()

function(init_debug)
    if (DEBUG)
        add_definitions(-DDEBUG=${DEBUG})
    endif ()
endfunction()

function(add_openmp)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        message(STATUS "Using OpenMP")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" PARENT_SCOPE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" PARENT_SCOPE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}" PARENT_SCOPE)

    endif ()
endfunction()

function(select_source_files)
    file(GLOB_RECURSE SOURCE_FILES
            "*.h"
            "*.cpp"
            "*.hpp"
            )

    set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
endfunction()

function(init_library library_name link_libraries)
    message("Adding library: ${library_name}")
    add_library(${library_name} ${SOURCE_FILES})

    target_link_libraries(
            ${library_name}
            ${link_libraries}
    )

    define_lib_includes()
endfunction()