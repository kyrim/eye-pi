cmake_minimum_required(VERSION 3.8)
project(eye-pi)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED)

find_package(OpenCV REQUIRED)

include_directories(/usr/include/)

file(GLOB SOURCE_FILES 
	"native/*.cpp" 
	"native/Camera/*.cpp" 
	"native/Camera/*.hpp" 
	"native/MotionDetection/*.hpp" 
	"native/MotionDetection/*.cpp" 
	"native/Utility/*.hpp" 
	"native/Utility/*.cpp")

#file(GLOB SOURCE_FILES "native/test.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC} "${CMAKE_SOURCE_DIR}/node_modules/streaming-worker-sdk")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS})