set(SOURCE_FILES
        NetMessage.cpp
        includes/NetMessage.h)

include_directories(includes)
add_library(Common STATIC ${SOURCE_FILES})
target_include_directories(Common PUBLIC includes)
target_include_directories(Common PRIVATE ..)