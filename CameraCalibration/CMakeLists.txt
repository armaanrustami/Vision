cmake_minimum_required(VERSION 2.8)
project(Assignment)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )

add_executable(ImageListCreator imagelist_creator.cpp)
add_executable(Calibration calibration.cpp)

target_link_libraries( ImageListCreator ${OpenCV_LIBS} )
target_link_libraries( Calibration ${OpenCV_LIBS} )
