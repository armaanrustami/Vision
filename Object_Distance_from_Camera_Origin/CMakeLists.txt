cmake_minimum_required(VERSION 2.8)
project(Assignment)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )

set(SOURCE_FILES main.cpp)
add_executable(Assignment ${SOURCE_FILES})
target_link_libraries( Assignment ${OpenCV_LIBS} )

add_custom_command(TARGET Assignment POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Assignment>/resources)
