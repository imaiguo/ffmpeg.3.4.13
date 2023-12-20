
cmake_minimum_required(VERSION 3.10)

project(ffdecode)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR D:/devtools/opencv.4.8.0)

if(MINGW)
    set(log4cplus_DIR D:/devtools/log4cplus.mingw64.2.1.0/lib/cmake/log4cplus)
else()
    set(log4cplus_DIR D:/devtools/log4cplus-2.1.0/lib/cmake/log4cplus)
endif()


find_package(log4cplus REQUIRED)
find_package(OpenCV REQUIRED)

aux_source_directory(src SRC)

include_directories(D:/devtools/ffmpeg.3.4.13/include)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC D:/devtools/ffmpeg.3.4.13/bin)

target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} log4cplus::log4cplusU avdevice avcodec avdevice avcodec avformat swscale avutil)

file( COPY config/log4cplus.properties DESTINATION ./config)

