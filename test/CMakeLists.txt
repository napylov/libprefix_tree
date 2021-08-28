cmake_minimum_required(VERSION 3.0)

project(libprefix_tree_test)

find_package( gtest )

find_library( GTEST gtest )
find_library( PTHREAD pthread )

set( CMAKE_CXX_FLAGS "-Wall ${BUILD_FLAGS} -std=c++17" )

set( TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/src )

set( TEST_UTILITY libprefix_tree_test )

set(
    TEST_SRC
    ${PREFIX_TREE_SRC}
    ${TEST_SRC_DIR}/test_prefix_tree.cpp
    ${TEST_SRC_DIR}/test_main.cpp
)


add_executable(
    ${TEST_UTILITY}
    ${TEST_SRC}
)


target_link_libraries(
    ${TEST_UTILITY}
    ${GTEST}
    ${PTHREAD}
)


add_test(
    NAME    ${TEST_UTILITY}
    COMMAND ${TEST_UTILITY}
)

