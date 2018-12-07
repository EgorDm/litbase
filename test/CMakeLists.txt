find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

function(init_test_project DEPENDENCIES)
    string(REPLACE "/" ";" CUR_DIRECTORY_TOKENS ${CMAKE_CURRENT_LIST_DIR})
    LIST(REVERSE CUR_DIRECTORY_TOKENS)
    LIST(GET CUR_DIRECTORY_TOKENS 0 TEST_PROJECT_NAME)

    message("Added tests: ${TEST_PROJECT_NAME}")
    project(${TEST_PROJECT_NAME})
    select_source_files()

    add_executable(${PROJECT_NAME} ${SOURCE_FILES})

    FOREACH(DEPENDENCY ${DEPENDENCIES})
        include_directories(${${DEPENDENCY}_INCLUDE_DIRS})
    ENDFOREACH(DEPENDENCY)

    target_link_libraries(${PROJECT_NAME}
            gtest
            gmock
            ${DEPENDENCIES})

    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
endfunction()

add_subdirectory(test_litaudiofile)
add_subdirectory(test_litsignal)