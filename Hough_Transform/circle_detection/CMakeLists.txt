cmake_minimum_required (VERSION 2.8)

# This is the name of the project
PROJECT( detectCircles  )

# This is for recursively searching for other directories.
# add_subdirectory (DirectoryName)

# sources to include
include_directories (.)
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE( detectCircles  main.cpp)
TARGET_LINK_LIBRARIES( detectCircles ${OpenCV_LIBS} )

