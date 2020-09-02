add_library(libecpb
    App.cpp
    Arguments.cpp
    RPC.cpp
    Session.cpp
)

target_link_libraries(libecpb Qt5::Core Qt5::Network)

add_executable(ecpb
  main.cpp
)

target_link_libraries(ecpb libecpb Qt5::Core Qt5::Network)
