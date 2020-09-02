find_package(Qt5 COMPONENTS Test REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_rpc
    TestRPC.cpp
    MockDevice.cpp
    ${PROJECT_SOURCE_DIR}/src/RPC.cpp
)

add_test(rpc test_rpc)

target_link_libraries(test_rpc libecpb Qt5::Test Qt5::Network)
