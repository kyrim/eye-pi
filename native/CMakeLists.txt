cmake_minimum_required(VERSION 3.8)
project(eye-pi)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED)

find_package( OpenCV REQUIRED )

include_directories(/usr/include/)

set(SOURCE_FILES
        MotionDetection/MotionDetector.cpp
        MotionDetection/MotionDetector.hpp
        MotionDetection/MotionFrame.cpp
        MotionDetection/MotionFrame.hpp
        Camera/CameraStreamer.cpp
		Camera/CameraStreamer.hpp
        main.cpp)

add_executable(eye-pi ${SOURCE_FILES})
target_link_libraries( eye-pi ${OpenCV_LIBS} )