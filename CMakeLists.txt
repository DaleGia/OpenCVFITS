cmake_minimum_required(VERSION 3.27)
project(OpenCVFITS LANGUAGES CXX)

add_executable(example examples/example.cpp)

############## OpenCV STUFF ########################################
find_package(OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries(example PRIVATE ${OpenCV_LIBS} )
##################################################################

############## cfitsio ############################################
target_link_libraries(example PRIVATE "cfitsio")
