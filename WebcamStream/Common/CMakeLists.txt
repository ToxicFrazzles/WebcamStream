set(SOURCE_FILES
        NetMessage.cpp
        includes/NetMessage.h
        Webcam.cpp includes/Webcam.h)
set(CommonDeps avcodec avdevice avutil avformat)

include_directories(includes)
find_library(AVCodec avcodec REQUIRED)
find_library(AVFormat avformat REQUIRED)
add_library(Common OBJECT ${SOURCE_FILES})
target_include_directories(Common PUBLIC includes)
target_include_directories(Common PRIVATE ${CommonDeps} ..)

set_property(GLOBAL PROPERTY CommonDepsProperty ${CommonDeps})