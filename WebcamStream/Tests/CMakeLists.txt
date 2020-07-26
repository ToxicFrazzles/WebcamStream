find_library(Common Common REQUIRED)

get_property(CommonDeps GLOBAL PROPERTY CommonDepsProperty)

add_executable(NetMessageTest NetMessage.cpp)
target_link_libraries(NetMessageTest Common ${CommonDeps})

add_executable(CaptureTest Capture.cpp)
target_link_libraries(CaptureTest Common ${CommonDeps})

add_executable(FormatTest TestCameraFormats.cpp)
target_link_libraries(FormatTest Common ${CommonDeps})
