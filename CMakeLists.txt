cmake_minimum_required(VERSION 3.0)

project(libprefix_tree)

#enable_testing()

#set( SRC_DIR ${PROJECT_SOURCE_DIR}/src )

#set( DOC_DIR ${PROJECT_SOURCE_DIR}/doc )
set( INC_DIR ${PROJECT_SOURCE_DIR}/include )

#set( TEST_SUBDIR test )
#set( TEST_DIR ${PROJECT_SOURCE_DIR}/${TEST_SUBDIR} )

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()


if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set( BUILD_FLAGS "-O3" )
else()
    set( BUILD_FLAGS "-g -O0 -DDEBUG_BUILD" )
endif()


set( CMAKE_CXX_FLAGS "-Wall ${BUILD_FLAGS} -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=0" )

#find_library( GTEST gtest )

include_directories( ${INC_DIR} )

set(
    PREFIX_TREE_SRC
    ${INC_DIR}/prefix_tree/prefix_tree.h
    ${INC_DIR}/prefix_tree/prefix_tree_map.h
)

add_library(
    prefix_tree
    INTERFACE
    ${PREFIX_TREE_SRC}
)

#target_link_libraries(
#    prefix_tree
#    ${EVENT}
#    ${GTEST}
#    ${PTHREAD}
#)

#add_subdirectory( test ${TEST_SUBDIR} )
#add_subdirectory( examples )

#set_property(TARGET prefix_tree PROPERTY CXX_STANDARD 17)

install(
    TARGETS prefix_tree
    LIBRARY DESTINATION lib
)

install(
    DIRECTORY ${INC_DIR}/prefix_tree
    PUBLIC_HEADER DESTINATION include
)
