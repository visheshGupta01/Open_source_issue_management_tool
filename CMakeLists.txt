cmake_minimum_required(VERSION 3.16)
project(IssueForge VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_library(issueforge_core src/csv.c src/validation.c)
target_include_directories(issueforge_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(issueforge_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(issueforge src/main.c)
target_link_libraries(issueforge PRIVATE issueforge_core)
target_compile_options(issueforge PRIVATE -Wall -Wextra -Wpedantic)

include(CTest)
if(BUILD_TESTING)
    add_executable(issueforge_tests tests/test_foundation.c)
    target_link_libraries(issueforge_tests PRIVATE issueforge_core)
    target_compile_options(issueforge_tests PRIVATE -Wall -Wextra -Wpedantic)
    add_test(NAME foundation_tests COMMAND issueforge_tests)
    set_tests_properties(foundation_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
